import random
import sys

def generate_game_of_life_input(width, height, output_file):
    with open(output_file, "w") as file:
        file.write(f"{width} {height}\n")
        for i in range(height):
            for j in range(width):
                if random.random() < 0.5:
                    file.write(f"{i} {j}\n")

if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Usage: python script.py <width> <height> <output_filename>")
        sys.exit(1)

    try:
        width = int(sys.argv[1])
        height = int(sys.argv[2])
        output_filename = sys.argv[3]
    except ValueError:
        print("Width and height must be integers.")
        sys.exit(1)

    generate_game_of_life_input(width, height, output_filename)
    print(f"Generated {width}x{height} Game of Life input and saved it to {output_filename}")
