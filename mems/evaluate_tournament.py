
res_file=["res_0.txt", "res_1000.txt", "res_100.txt", "res_10.txt", "res_2.txt"]


results=[]

for file_name in res_file:
    scores=[];
    with open(file_name, 'r') as f:
        for line in f:
            if line.startswith("Score:"):
               scores.append(int(line[7:]))
    if len(scores)!=10000:
        print "Warning: ", file_name, "has not exactly 10000 scores"
    
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
    
 
print "expectation of 0 is 115.911";
  
def stats(scores):
    print "negative score: ", len([score for score in scores if score<0])
    print "score>200: ", len([score for score in scores if score>200])
    mean=sum(scores)/float(len(scores))
    m2=sum([i*i for i in scores])/float(len(scores))
    return (mean, m2-mean*mean)
 
   
import math

n=len(res_file)
for i in xrange(n):
    print res_file[i], ":"
    mean, var=stats(results[i])
    N=len(results[i])
    print "Average score:", mean, "+/-", math.sqrt(var/(N-1)), "[sigma=",math.sqrt(var),"]" 
    print "Scores with others:"
    for j in xrange(n):
      if i!=j:
          w,r,l=compare(results[i], results[j]) 
          print res_file[j],":\t %.1f:%.1f (+%d =%d -%d)"%(w+.5*r, 0.5*r+l, w,r,l) 



import matplotlib.pyplot as plt
import numpy as np
hist, bins = np.histogram(results[1], range(-20,250))
width = 1.0 * (bins[1] - bins[0])
center = (bins[:-1] + bins[1:]) / 2
plt.bar(center, hist, align='center', width=width)
plt.title('histogram of exact results')
plt.show()           
