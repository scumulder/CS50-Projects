import cs50

while True:
  print("Height: ")
  height = cs50.get_int()
  if height < 0 or height > 23:
      break
  width = height + 1
  for i in range(height):
     for col in range(i, height-1):
        print(" ", end="")
     for col in range(height - i - 1, width):
        print("#", end="")
     print()
