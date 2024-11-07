ref_string = [0, 2, 1, 6, 4, 0, 1, 0, 3, 1, 2, 1]
win = 0
hit = 0
miss = 0
res = []
last_used = []

def find_lru():
    min_index = 0
    for i in range(1, win):
        if last_used[i] < last_used[min_index]:
            min_index = i
    return min_index

def lru(rf, length):
    global res, last_used, hit, miss
    res = [-1] * win
    last_used = [-1] * win
    hit = 0
    miss = 0

    for j in range(length):
        hit_flag = False
        for k in range(win):
            if res[k] == rf[j]:
                hit += 1
                last_used[k] = j
                hit_flag = True
                break
        if not hit_flag:
            miss += 1
            empty_found = False
            for k in range(win):
                if res[k] == -1:
                    res[k] = rf[j]
                    last_used[k] = j
                    empty_found = True
                    break
            if not empty_found:
                lru_index = find_lru()
                res[lru_index] = rf[j]
                last_used[lru_index] = j

    print(f"No of hits (LRU): {hit}")
    print(f"No of misses (LRU): {miss}")

def find_optimal(rf, length, current_index):
    max_index = -1
    farthest = current_index
    for i in range(win):
        j = current_index + 1
        while j < length:
            if res[i] == rf[j]:
                if j > farthest:
                    farthest = j
                    max_index = i
                break
            j += 1
        if j == length:
            return i
    return max_index

def optimal(rf, length):
    global res, hit, miss
    res = [-1] * win
    hit = 0
    miss = 0

    for j in range(length):
        hit_flag = False
        for k in range(win):
            if res[k] == rf[j]:
                hit += 1
                hit_flag = True
                break
        if not hit_flag:
            miss += 1
            empty_found = False
            for k in range(win):
                if res[k] == -1:
                    res[k] = rf[j]
                    empty_found = True
                    break
            if not empty_found:
                optimal_index = find_optimal(rf, length, j)
                res[optimal_index] = rf[j]

    print(f"No of hits (Optimal): {hit}")
    print(f"No of misses (Optimal): {miss}")

def display():
    print("Pages in window are:")
    for i in range(win):
        print(res[i])

def main():
    global win
    win = int(input("Enter window size: "))
    length = len(ref_string)

    print("\nBy LRU algorithm:")
    lru(ref_string, length)
    display()

    print("\nBy Optimal Algorithm:")
    optimal(ref_string, length)
    display()

if __name__ == "__main__":
    main()
