import sys

def convert_gz_to_c_array(filename):
    with open(filename, 'rb') as f:
        data = f.read()

    c_array = ', '.join(f'0x{byte:02x}' for byte in data)
    array_name = filename.replace('.', '_')

    print(f"const unsigned char {array_name}[] = {{")
    print(c_array)
    print(f"}};")
    print(f"const unsigned int {array_name}_len = {len(data)};")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python gz_to_c.py <filename.gz>")
    else:
        convert_gz_to_c_array(sys.argv[1])
