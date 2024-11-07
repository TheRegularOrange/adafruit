filesize = []
blocksize = []
res = []

def firstfit():
    global res
    res = [-1] * len(filesize)

    for i in range(len(filesize)):
        for j in range(len(blocksize)):
            if filesize[i] <= blocksize[j] and j not in res:
                res[i] = j
                break
    display("First Fit")

def bestfit():
    global res
    res = [-1] * len(filesize)

    for i in range(len(filesize)):
        best_index = -1
        min_diff = float('inf')
        for j in range(len(blocksize)):
            diff = blocksize[j] - filesize[i]
            if diff >= 0 and diff < min_diff and j not in res:
                min_diff = diff
                best_index = j
        if best_index != -1:
            res[i] = best_index
    display("Best Fit")

def worstfit():
    global res
    res = [-1] * len(filesize)

    for i in range(len(filesize)):
        worst_index = -1
        max_diff = -1
        for j in range(len(blocksize)):
            diff = blocksize[j] - filesize[i]
            if diff >= 0 and diff > max_diff and j not in res:
                max_diff = diff
                worst_index = j
        if worst_index != -1:
            res[i] = worst_index
    display("Worst Fit")

def nextfit():
    global res
    res = [-1] * len(filesize)
    last_allocated = 0

    for i in range(len(filesize)):
        allocated = False
        for j in range(len(blocksize)):
            index = (last_allocated + j) % len(blocksize)
            if filesize[i] <= blocksize[index] and index not in res:
                res[i] = index
                last_allocated = index
                allocated = True
                break
        if not allocated:
            last_allocated = 0
    display("Next Fit")

def getinput():
    n1 = int(input("Enter number of blocks: "))
    n2 = int(input("Enter number of files: "))

    for n in range(n1):
        inp = int(input(f"Enter Block {n} size: "))
        blocksize.append(inp)

    for n in range(n2):
        inp = int(input(f"Enter File {n} size: "))
        filesize.append(inp)

def display(t):
    print("\n" + t)
    headers = ["File Index", "File Size", "Block Index", "Block Size (After Allocation)"]
    print(f"{headers[0]:<12} | {headers[1]:<10} | {headers[2]:<12} | {headers[3]:<20}")
    print("-" * 60)

    for i, file in enumerate(filesize):
        block_index = res[i]
        allocated_block_size = blocksize[block_index] if block_index != -1 else '-'
        print(f"{i:<12} | {file:<10} | {block_index:<12} | {allocated_block_size:<20}")

# Get input and run each allocation strategy
getinput()
firstfit()
bestfit()
worstfit()
nextfit()
