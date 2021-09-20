import os
from problem import Problem
from jinja2 import Environment
from jinja2.loaders import FileSystemLoader

class SceneHandler:
    def __init__(self, problem: Problem) -> None:
        self.problem = problem
    
    def ToPDDL(self, pathToFile):
        '''processes problem data to generate a PDDL domain file'''
        #Initiating instance of Problem
        self.problem.InitStates()

        #Extracting relevant data for PDDL problem file
        pddl_data = self.problem.GenerateDataForPDDL()
        
        #Utilizing template engine to generate pddl file
        env = Environment(loader=FileSystemLoader("pddl/templates"),
            trim_blocks=True,
            lstrip_blocks=True
        )

        template = env.get_template("pfile.pddl")
        output_from_parsed_template = template.render(data=pddl_data)

        # to save the results
        with open("{}/{}.pddl".format(pathToFile, pddl_data["pname"]), "w") as fh:
            fh.write(output_from_parsed_template)
            print(f'File {fh.name} written successfully')

        return fh.name
    
    @staticmethod
    def Plan(plannerPath, dfilePath, pfilePath, planner = "lpg-td"):
        '''runs the planner on the given paths and returns a plan file path'''
        pfileDir = os.path.dirname(pfilePath)
        # os.chdir(dfileDir)
        if planner == "lpg-td":
            output_filename = str(pfileDir + '/' + 'plan_' + os.path.basename(pfilePath))
            try:
                os.system(f'{plannerPath} -o {dfilePath} -f {pfilePath} -quality -restarts 5')
                #-out {output_filename}
                #os.remove(output_filename) #Removing a redundant file
            except FileNotFoundError:
                raise Exception("Error in solving PDDL problem")
            return str(output_filename + "_1" +".SOL"), 13, None
    

