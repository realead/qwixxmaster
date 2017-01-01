
with open('tournament_2_5.in','w') as f:
    for i in xrange(10000):
        f.write('set 2 5 2 5 2 9 2 9 0\n')
        f.write('autoplay %d\n'%i)
        
