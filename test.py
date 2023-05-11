import csv

# with open('test.txt', 'r') as in_file:
#     stripped = (line.strip() for line in in_file)
#     print(stripped)

#     lines = (line.split(",") for line in lines if line)
#     print(lines)

#     with open('test.csv', 'w') as out_file:
#         writer = csv.writer(out_file)
#         print(lines)
#         writer.writerow(lines)




lines=["hello,my,mas,opa","my,man"]
# decoded_bytes = lines.split(",")
# print(decoded_bytes)
lines  = (line.strip() for line in lines)
with open("test.csv","w", newline='') as f:
    writer = csv.writer(f)
    for lines in lines:
        decoded_bytes = lines.split(",")

        writer.writerow(decoded_bytes)