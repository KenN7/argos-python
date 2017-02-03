from LedColor import LedColor

l = LedColor(red_channel_value = 23, green_channel_value = 23, blue_channel_value = 23)

l2 = LedColor(value=int("10111000101110001011100000000", 2))

print(l == l2)
print(l)
print(l2)