# Script to convert hex code colours to our representation.

loop = True

while loop:
    # Get hex code
    hex = input("Enter hex code of colour: ")
    hex = bytes.fromhex(hex)
    r = hex[0]
    g = hex[1]
    b = hex[2]

    r01 = round(hex[0] / 255, 3)
    g01 = round(hex[1] / 255, 3)
    b01 = round(hex[2] / 255, 3)

    # Output RGBA
    rgba = "RGBA: [" + str(r01) + ", " + str(g01) + ", " + str(b01) + ", 1]"
    print(rgba)

    # Calculate and output HSLA
    h = 0
    s = 0
    l = 0
    maxChannel = max(r01, g01, b01)
    minChannel = min(r01, g01, b01)
    chroma = maxChannel - minChannel

    # Hue
    if abs(chroma) > 1e-6:
        if (abs(maxChannel - r01) < 1e-6):
            h = 60.0 * (g01 - b01) / chroma / 360.0
        elif (abs(maxChannel - g01) < 1e-6):
            h = 60.0 * (2 + (b01 - r01) / chroma) / 360.0
        else:
            h = 60.0 * (4 + (r01 - g01) / chroma) / 360.0
    # Lightness
    l = (maxChannel + minChannel) / 2.0
    # Saturation
    if l > 0 and l < 1:
        s = (maxChannel - l) / min(l, 1 - l)

    h = round(h, 3)
    s = round(s, 3)
    l = round(l, 3)
    hsla = "HSLA: [" + str(h) + ", " + str(s) + ", " + str(l) + ", 1]"
    print(hsla)

    print("")
    loop = input("Continue? (Y/n): ") != "n"