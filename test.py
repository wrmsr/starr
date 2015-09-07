import fcntl
with open('/dev/starr', 'rb') as f:
    for _ in xrange(3):
        fcntl.ioctl(f.fileno(), 0, 0)

