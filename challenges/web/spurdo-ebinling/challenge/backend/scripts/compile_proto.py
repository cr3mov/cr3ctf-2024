from pathlib import Path
from shutil import rmtree
from subprocess import run


messages_dir = Path(__file__).resolve().absolute().parent.parent / 'messages'
out_dir = Path(__file__).resolve().absolute().parent.parent / 'server' / 'proto'

if out_dir.is_dir():
    rmtree(out_dir)

out_dir.mkdir()

for message in messages_dir.glob('*.proto'):
    run([
        messages_dir / 'protoc.exe',
        '--pyi_out', str(out_dir),
        '--proto_path', str(messages_dir.absolute()),
        '--python_out', str(out_dir),
        str(message.absolute())
    ])

open(out_dir / '__init__.py', 'w').close()
