import os, shutil
from time import sleep
import random 

from problem import Problem
from ai2thor.controller import Controller
from tasks import ClearDishes, SliceTomato, MakeToast, ServeCoffee, CookEgg, SetTable
import scene_info as si
from pddl2scene import PlanHandler
from scene2pddl import SceneHandler

kitchens = [f"FloorPlan{i}" for i in range(1, 31)]
rand_scene = random.choice(kitchens)

controller = Controller(
    agentMode="default",
    visibilityDistance = si.visibilityDistance,
    scene=rand_scene,

    # step sizes
    gridSize=0.25,
    snapToGrid=True,
    rotateStepDegrees=90,

    # image modalities
    renderDepthImage=False,
    renderInstanceSegmentation=False,

    branch="main",


    #camera properties
    width=768,
    height=432,
    fieldOfView=140
)

# tasks = [ServeCoffee(),
#          ServeEgg()]

tasks = [MakeToast(), CookEgg()]

domain_filename = "pddl/robochef.pddl"
problems_path = "pddl/problems"
planner_path = "planners/LPG-td-1.4/lpg-td"
max_actions = 1000


##clean old data
if os.path.exists(problems_path):
    shutil.rmtree(problems_path)
os.mkdir(problems_path)


#model the problem
problem = Problem(controller, tasks)

#generate a PDDL problem file
problem_filename = SceneHandler(problem).ToPDDL(problems_path)

#use planner to get a solution
plan_filepath, l_from, l_to = SceneHandler.Plan(planner_path, domain_filename, problem_filename)

plan_handler = PlanHandler(plan_filepath, problem, l_from, l_to)
#execute actions
for i in range(max_actions):
    action = plan_handler.parse_line()
    if action is not None:
        event = action.execute()
        sleep(0.5)
        if event is not None:
            Problem.PrintLastActionStatus(event, action.args)
            if not event.metadata["lastActionSuccess"]:
                problem.controller.step('Pass')
                input("Mission failed! Press Enter to continue...")
                break
    else:
        problem.controller.step('Pass')
        input("Mission finished! Press Enter to continue...")
        break


    
    
    



