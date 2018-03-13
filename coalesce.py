import os
import shutil


def coalesce(source_dir, output_dir, file_size):
    files = [os.path.join(source_dir, f) for f in os.listdir(source_dir)
             if os.path.isfile(os.path.join(source_dir, f))]
    create_directory(output_dir)
    coalesced_data = bytearray()
    count = 1
    for f in files:
        with open(f, "rb") as binary_file:
            data = bytearray(binary_file.read())
            for _byte in data:
                coalesced_data.append(_byte)
                if len(coalesced_data) == file_size:
                    write_to_file(output_dir, count, coalesced_data)
                    coalesced_data = bytearray()
                    count += 1
    write_to_file(output_dir, count, coalesced_data)


def write_to_file(output_dir, count, coalesced_data):
    output_file = os.path.join(output_dir, str(count))
    new_file = open(output_file, "wb+")
    new_file.write(coalesced_data)


def create_directory(output_dir):
    if os.path.exists(output_dir):
        shutil.rmtree(output_dir)
    os.makedirs(output_dir)




coalesce('/Users/dennis/coding/compressibility/winter-war-en.wikipedia.org-docs',
         '/Users/dennis/coding/compressibility/coalesce-output', 500000)
