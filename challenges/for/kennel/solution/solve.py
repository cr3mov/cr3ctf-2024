import cv2 as cv

base_path = "../challenge/taksa.png"


def xor_strings(str1, str2):
    result = ""
    for char1, char2 in zip(str1, str2):
        xor_result = ord(char1) ^ ord(char2)  # XOR the ASCII codes of characters
        result += chr(xor_result)  # Convert the result back to a character
    return result


def extract_data(src, data_pixels):
    # if you check exiftool, you will see that an image uses color type RGB
    # and bit depth is 16, by default opencv converts it into 8 bit picture, 
    # so we have to specify IMREAD_ANYEDEPTH + ANYCOLOR, otherwise you will 
    # get rounded colors and won't get needed data with xor 

    base_img = cv.imread(base_path, cv.IMREAD_ANYDEPTH | cv.IMREAD_ANYCOLOR)
    bwidth, bheight, bchannels = base_img.shape

    data_img = cv.imread(src, cv.IMREAD_ANYDEPTH | cv.IMREAD_ANYCOLOR)
    dwidth, dheight, dchannels = data_img.shape

    if dwidth != bwidth or dheight != bheight or dchannels != bchannels:
        raise ValueError("Image dimensions mismatch")

    # hehe-haha, it's RGB but 2 bytes per color
    bytes = data_pixels // 4
    decrypted_data = [0] * bytes

    # the data is in BGR format, because we used cv.imread
    for x in range(0, data_pixels, 4):
        enc_data = 0
        for b in range(0, 4):
            _, bg, br = base_img[0][x + b]
            _, dg, dr = data_img[0][x + b]

            data_red = (br ^ dr) & 1
            data_green = (bg ^ dg) & 1

            enc_data = (enc_data << 2) | (data_red << 1) | (data_green)
        decrypted_data[x // 4] = enc_data

    return (''.join(map(chr, decrypted_data)))


# we don,t care about first beacon, it's just a file creation
b1 = extract_data("../challenge/charlie.png", 132)
b2 = extract_data("../challenge/my_dog.png", 132)
b3 = extract_data("../challenge/cooper.png", 132)

txt = b2.split('|')[1]
key = b3.split('|')[1]

print(xor_strings(txt, key))
