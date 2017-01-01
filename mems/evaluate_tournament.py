
res_file=["res0_2_5.txt", "res2_2_5.txt", "res10_2_5.txt", "res100_2_5.txt"]


results=[]

for file_name in res_file:
    scores=[];
    with open(file_name, 'r') as f:
        for line in f:
            if line.startswith("Score:"):
               scores.append(int(line[7:]))
    if len(scores)!=1000:
        print "Warning: ", file_name, "has not exactly 1000 scores"
    
    results.append(scores)



def compare(first, second):
   lost, remis, won=0,0,0
   for x,y, in zip(first, second):
        if x==y:
           remis+=1
        if x<y:
           lost+=1
        if x>y:
           won+=1
   return (won, remis, lost)
    
 
print "expectation of 0 is 106.222";
   
n=len(res_file)
for i in xrange(n):
    print res_file[i], ":"
    print "Average score:", sum(results[i])/float(len(results[i]))
    print "Scores with others:"
    for j in xrange(n):
      if i!=j:
          w,r,l=compare(results[i], results[j]) 
          print res_file[j],":\t %.1f:%.1f (+%d =%d -%d)"%(w+.5*r, 0.5*r+l, w,r,l)    
   
              
