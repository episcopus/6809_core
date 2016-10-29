f = open("opcode_table.c", mode='w')
f.write("const struct opcode_def opcode_table[] = {\n")

for i in range(256):
    f.write("    { \"NOTIMPL\", 0x%0X, 2, INHERENT, notimpl },\n" % (i))

f.write("};\n")
f.close()
