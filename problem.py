from copy import Error
from scene_info import Object
from scene_info import Scene
from predicates import Predicate, interactable, near
from tasks import Task
from random import randrange

class Problem:

    def __init__(self, scene: Scene, subtasks) -> None:
        self.scene = scene
        self.subtasks = subtasks
        self.controller = scene.controller
        self.objects = {} #types -> objects
        self.predicates = []
        self.goal_predicates = []

    #Extract the states based on the required subtasks
    def InitStates(self):
        if len(self.subtasks) < 1:
            raise ValueError("You have to add tasks to the problem")
        assert all([isinstance(sub,Task) for sub in self.subtasks])

        ##Combining subtasks to common predicates and objects##
        n_subtasks = len(self.subtasks)

        #Merging occurences in both types and predicates
        task_types = [sub.types for sub in self.subtasks]
        task_init_predicates = [sub.init_predicates for sub in self.subtasks]
        task_goal_predicates = [sub.goal_predicates for sub in self.subtasks]

        ##Maintaining a list of relevant objects##
        allObjects = self.scene.objects
        relevants = list(filter(lambda x: x.metadata["type"] in sum(task_types, []), allObjects))

        types2objects = {} #Assumes to have one object of each type
        for i in range(n_subtasks):

            #Matching a type to an actual object
            for type_str in task_types[i]:
                try:
                    if type_str not in types2objects.keys():
                        assignedObj = next(x for x in relevants if x.metadata["type"] == type_str)
                        relevants.remove(assignedObj)
                        types2objects[type_str] = assignedObj
                        #self.objects.append(assignedObj)
                    else:
                        assignedObj = types2objects[type_str]
                except StopIteration:
                    print(f'Object of type {type_str} not found')
                    raise
            
            #Creating a predicate entity from objects
            for pred_str in task_init_predicates[i]:
                pred = Predicate.parse(pred_str, types2objects)
                #Check the initial state is fufilled
                correct = not(pred.value()) if pred_str.split()[0] == 'not' else pred.value()
                if not correct:
                    raise Exception("Incorrect initial state")
                self.predicates.append(pred)

            for pred_str in task_goal_predicates[i]:
                pred = Predicate.parse(pred_str, types2objects)
                self.predicates.append(pred)
                self.goal_predicates.append(pred)

        self.objects = types2objects 
        
        #Adding other essential predicates
        for obj in self.objects.values():
            self.predicates.append(near(obj))
            self.predicates.append(interactable(obj))

    def GetTruePredicates(self):
        '''returns list of true predicates'''
        return list(filter(lambda x: x.value(), self.predicates))
    
    def GenerateDataForPDDL(self):
        data = {"pname": "problem_{}".format(randrange(999999)),
        "objects": self.objects,
        "init": [init_pred.unparse() for init_pred in self.predicates if init_pred.value() == True],
        "goal": [goal_pred.unparse() for goal_pred in self.goal_predicates]}

        return data
