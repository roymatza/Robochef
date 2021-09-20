from actions import Action
from scene_info import Object

class PlanHandler:
    def __init__(self, pathToFile:str, problem, l_from, l_to) -> None:
        self.problem = problem
        self.path = pathToFile
        try:
            with open(pathToFile) as plan:
                if l_to is None:
                    self.lines = plan.readlines()[l_from:] #Get relevant lines
                else:
                    self.lines = plan.readlines()[l_from: l_to]
        except FileNotFoundError:
            raise Exception("Failed to find a problem file.")

        self.actions = []

    def parse_line(self):
        if self.lines:
            line = self.lines.pop(0)
                
            #Parsing the line
            words = line.split()
            time = float(words[0].replace(':', ''))
            cost = float(words[-1].replace('[','').replace(']',''))
            action = words[1:-1]
            action[0] = action[0].replace('(','')
            action[-1] = action[-1].replace(')','')

            return Action.parse(str.lower(' '.join(action)), self.problem)
        else:
            return None
    
    def print_plan(self):
        with open(self.path) as plan:
            print(plan.read())




