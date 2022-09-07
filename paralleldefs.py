# compute 
def factorial(number):  
    f = 1
    for i in range(2, number+1):
        f *= i
    return f
 
# synchronous I/O
def io_from_devnull(number):
    with open("/dev/null", "rb") as fh:
        for i in range(number):
            fh.read(1)
    return number        