
with open('tournament.in','w') as f:
    for i in xrange(10000):
        f.write('restart\n')
        f.write('autoplay %d\n'%i)
        
