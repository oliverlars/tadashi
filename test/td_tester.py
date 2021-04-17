# tadashi test harness
# april 2021
#

import subprocess

#rc = subprocess.run(["tadashi", "-file", "my_code_08.td",],  stdout=subprocess.PIPE, stderr=subprocess.PIPE)

#print("Return code was", rc.returncode)

#print(str(rc.stdout))

programs = [("my_code_01.td", ["RA: 0", "RB: 0", "RC: 55000", "RD: 12" ], [None,]),
            ("my_code_02.td", ["RA: 0", "RB: 10", "RC: 55000", "RD: 42" ],["i: 10", "j: 2", "k: 20"]),
            ("my_code_03.td", ["RA: 0", "RB: 10", "RC: 55000", "RD: 115" ],["the_max: 10", "j: 2", "k: 200", "kj: 10"]),
            ("my_code_04.td", ["RA: 0", "RB: 52", "RC: 55000", "RD: 371" ],["pack: 0", "pack[13]: 13"]),
            ("my_code_05.td", ["RA: 0", "RB: 52", "RC: 55000", "RD: 147" ],["sieve: 8193", "sieve[29]: 29", "sieve[30]: -1", "elements: 52", "step: 52"]),
            ("my_code_06.td", ["RA: 0", "RB: 0", "RC: 55000", "RD: 0" ],["div_temps: 8193", "div_temps[0]: 0", "div_temps[7]: 1"]),
            ("my_code_07.td", ["RA: 0", "RB: 0", "RC: 55000", "RD: 0" ],["div_temp: 3", "x: 110", "y: 52", "mod: 0"]),
            ("my_code_08.td", ["RA: 0", "RB: 5", "RC: 55000", "RD: 78" ],["ans: 25"]),
            ]
           
def run_test(tadashi_file):
    rc = subprocess.run(["tadashi", "-file", tadashi_file,],  stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    return rc

def extract_asm_code(prog_output):
    asm_code = prog_output.split("registers and their final state")[0]
    return asm_code
    
def extract_registers(prog_output):
    registers = prog_output.split("========================================")[1]
    registers = registers.replace("entry() variables and their final state", "")

    return registers
    
def extract_variables(prog_output):
    variables = prog_output.split("========================================")[2]

    return variables
    
for tadashi_file, results_regs, results_vars in programs:
    
    test_return = run_test(tadashi_file)
    
    if test_return.returncode == 0:
    
        prog_output = str(test_return.stdout)
        asm_code = extract_asm_code(prog_output)
        registers = extract_registers(prog_output)
        variables = extract_variables(prog_output)
        
        print("+"*80)
        print(f"\t{tadashi_file} ran")
        print("+"*80)
        print("registers ==================================")
        for result in results_regs:         
            if result in str(registers):
                print(f"passed - {result}")
            else:
                print(f"failed - {result}")
                
        print("variables ==================================")
        for variable in results_vars:
            if variable is not None and str(variables) is not None:
                if variable in str(variables):
                    print(f"passed - {variable}")
                else:
                    print(f"failed - {variable}")
                
                
                
    else:
        print(f"{tadashi_file} failed to execute")
    