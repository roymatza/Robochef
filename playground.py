import os, shutil

from ai2thor.server import Event
from problem import Problem
from ai2thor.controller import Controller
from tasks import ServeCoffee, ServeEgg
import scene_info as si
from pddl2scene import PlanHandler
from scene2pddl import SceneHandler

controller = Controller(
    agentMode="default",
    visibilityDistance = si.visibilityDistance,
    scene="FloorPlan3",

    # step sizes
    gridSize=0.25,
    snapToGrid=True,
    rotateStepDegrees=90,

    # image modalities
    renderDepthImage=False,
    renderInstanceSegmentation=False,

    # camera properties
    # width=960,
    # height=540,
    # fieldOfView=100
)


tasks = [ServeCoffee(),
         ServeEgg()]
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
        Problem.PrintLastActionStatus(event)
    else:
        break



