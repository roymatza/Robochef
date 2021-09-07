from actions import Action
from scene_info import Object

class PlanHandler:
    def __init__(self, pathToFile:str, problem, l_from, l_to) -> None:
        self.problem = problem
        try:
            with open(pathToFile) as plan:
                self.lines = plan.readlines()[l_from: l_to] #Get relevant lines
        except FileNotFoundError:
            raise Exception("Failed to create a problem file.")

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



