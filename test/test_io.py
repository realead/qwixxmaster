
##process:
import subprocess

class ProcessCallError(Exception):
      def __init__(self, message, returncode):
        Exception.__init__(self, message)
        self.returncode=returncode
        


class CallResult:
    def __init__(self, out, err, returncode):
        self.stdout=out
        self.stderr=err
        self.returncode=returncode


def execute_process(command, ok_code=[0]): 
    df = subprocess.Popen(command, stdout=subprocess.PIPE,  stderr=subprocess.PIPE)        
    output, err = df.communicate()
    code=df.returncode
    if code not in ok_code:
        raise ProcessCallError("Process returned unexpected code {0}".format(code), code)
    return CallResult(output, err, code)
    
    
#find test cases:
import glob

input_files=glob.glob("test/cases/*.in")


wrong=0
for in_data in input_files:
    result=execute_process(["bin/quixxmaster","<",in_data])
    with open(in_data[0:-2]+'ans', 'r') as myfile:
        expected=myfile.read();
    if expected!=result.stdout:
        print "**************************************"
        print "**************************************"
        print "Failed in test case :",in_data
        print "Expected:", expected
        print "Received:", result.stdout 
        print "**************************************"
        print "**************************************\n\n"
        wrong+=1
        
if not wrong:
    print "All", len(input_files), "ok"
else:
    print wrong,"out of",len(input_files),"wrong!"
    exit(1)
