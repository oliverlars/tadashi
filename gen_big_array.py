with open("test.txt", "w") as f:
    for i in range(0, 100):
        f.write("memory[" + str(i) + "] = 532452;\n")
