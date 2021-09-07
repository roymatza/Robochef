from copy import Error
from scene_info import Scene
from scene_info import Object
from problem import Problem
import random

class Action:
    #names of corresponding actions in pddl
    actionsDict = {
        "make-interactable": "make_interactable",
        "move-robot": "move_robot",
        "pickup": "pickup",
        "place": "place",
        "put": "place",
        "break-egg": "slice_object",
        "cook": "cook",
        "turn-on": "toggle_on",
        "make-coffee": "make_coffee",
        "turn-off": "toggle_off"
    }
    
    def __init__(self, problem: Problem, *obj_args) -> None:
        assert (all([isinstance(arg,Object) for arg in obj_args]) and len(obj_args) > 0) 
        self.scene = problem.scene
        self.controller = problem.controller
        self.objects = problem.objects.values()
        self.args = obj_args
        
    
    def execute(self):
        pass
    

    @staticmethod
    def parse(action_str: str, problem: Problem):
        action_words = action_str.split()
        
        action_name = action_words[0]
        action_args =  action_words[1:]
        len_args = len(action_words) - 1

        problem_objects = problem.objects.values()
        action_objects = []
        for arg_str in action_args:
            try:
                action_objects.append(next(filter(lambda x: x.metadata["name"] == arg_str, problem_objects)))
            except StopIteration:
                raise Exception(f'Object {arg_str} not found')
        
        action = globals()[Action.actionsDict[action_name]]
        return action(problem, *action_objects) 

class make_interactable(Action):
    '''change robot from one and pose to another and do some actions in order to make an object visible'''
    def __init__(self, problem, *obj_args) -> None:
        assert len(obj_args) == 1
        super().__init__(problem, *obj_args)
    
    def __reach(self, obj):
        relevant_poses = self.controller.step(
            action="GetInteractablePoses",
            objectId= obj.metadata["id"],
            standings=[True]
        ).metadata["actionReturn"]

        #In case the object can be made visible
        if len(relevant_poses) > 0:
            random_pose = random.choice(relevant_poses)
            self.controller.step(
                action="TeleportFull",
                **random_pose
            )
            return True
        else:
            return False
    
    def execute(self):
        dest_obj = self.args[0]
        if self.__reach(dest_obj):
            return self.controller.last_event
        #In case the object is not reachable
        else:     
            #Find all parent receptacles
            parents = [p_ob for p_ob in self.scene.objects if p_ob.metadata["id"] in dest_obj.metadata["parentReceptacles"]]
            for p in parents:
                #Go near them and try to open them
                if p.metadata["openable"]:
                    no_open = True
                    while self.__reach(p) and no_open:
                        no_open = not(self.controller.step(
                            action="OpenObject",
                            objectId=p.metadata["id"],
                            openness=1,
                            forceAction=False
                        ).metadata["lastActionSuccess"])
                #Check if now the required object is reachable
                if self.__reach(dest_obj):
                    return self.controller.last_event
            
            #Try to change pose
            for i in range(10):
                if self.__reach(dest_obj):
                    return self.controller.last_event

        raise Exception(f'object {dest_obj.metadata["name"]} is not reachable')

class move_robot(Action):
    '''move robot towards an object'''
    def __init__(self, problem, *obj_args) -> None:
        assert len(obj_args) == 2
        super().__init__(problem, *obj_args)
    
    def execute(self):
        return self.controller.step(action="Done")

class pickup(Action):
    '''pickup an object that is interactable to the robot'''
    def __init__(self, problem, *obj_args) -> None:
        assert len(obj_args) == 1
        super().__init__(problem, *obj_args)
    
    def execute(self):
        obj = self.args[0]
        if not obj.metadata["pickupable"]:
            raise Exception(f'object {obj.metadata["name"]} is not pickupable')


        event = self.controller.step(
            action="PickupObject",
            objectId=obj.metadata["id"],
            forceAction=False,
            manualInteract=False
        )

        return event
    
class place(Action):
    '''place an object onto a surface'''
    def __init__(self, problem, *obj_args) -> None:
        assert len(obj_args) == 2
        super().__init__(problem, *obj_args)
    
    def execute(self):
        to_place, surface = self.args[0], self.args[1]

        if not to_place.metadata["isPickedUp"]:
            Exception(f'object {to_place.metadata["name"]} is not picked up')
        if not surface.metadata["interactable"]:
            Exception(f'object {surface.metadata["name"]} is not interactable')
            
            # Exception(f'object {to_place.metadata["name"]} can\'t be placed on {surface.metadata["name"]}')

        event = self.controller.step(
            action="PutObject",
            objectId=surface.metadata["id"],
            forceAction=True,
            placeStationary=True
        )
        
        return event

class slice_object(Action):
    '''move robot towards an object'''
    def __init__(self, problem, *obj_args) -> None:
        assert len(obj_args) == 1 or len(obj_args) == 2
        super().__init__(problem, *obj_args)
    
    def execute(self):
        obj = self.args[0]
        if not obj.metadata["sliceable"]:
            raise Exception(f'object {obj.metadata["name"]} can\'t be sliced')

        event = self.controller.step(
            action="SliceObject",
            objectId=obj.metadata["id"],
            forceAction=False
        )

        if event.metadata["lastActionSuccess"]:
            if obj.metadata["type"] == "egg":
                name, new_id = obj.metadata["name"], obj.metadata["id"] + "|EggCracked_0"
                obj_to_update = next(filter(lambda x: x.metadata["name"] == name, self.objects))
                new_metadata = next(filter(lambda x: x["objectId"] == new_id, self.controller.last_event.metadata["objects"]))
                obj_to_update.fromScene(new_metadata)
                obj_to_update.metadata["name"], obj_to_update.metadata["type"], obj_to_update.metadata["id"] = name, "egg", new_id

        return event

class cook(Action):
    '''cook an object'''
    def __init__(self, problem, *obj_args) -> None:
        assert len(obj_args) == 3
        super().__init__(problem, *obj_args)
    
    def execute(self):
        to_cook = self.args[0]
        if not to_cook.metadata["cookable"]:
            raise Exception(f'Object {to_cook.metadata["name"]} is not cookable')

        if not to_cook.metadata["isCooked"]:
            event = self.controller.step(
                action="CookObject",
                objectId=to_cook.metadata["id"],
                forceAction=False)
        
        return self.controller.last_event
        
class toggle_on(Action):
    '''turn on a device'''
    def __init__(self, problem, *obj_args) -> None:
        assert len(obj_args) == 1
        super().__init__(problem, *obj_args)
    
    def execute(self):
        obj = self.args[0]

        if obj.metadata["type"] == "stoveburner":
            knobs = filter(lambda x:x.metadata["type"] == "stoveknob", self.scene.objects)
            for knob in knobs:
                knob_id = knob.metadata["id"]
                self.controller.step(
                action="ToggleObjectOn",
                objectId=knob_id,
                forceAction=False)
                # stoveburner_toggled = next(filter(lambda x: x["objectId"] == obj.metadata["id"], self.controller.last_event.metadata["objects"]))["isToggled"]
                 
        else:
            if not obj.metadata["isToggled"]:
                self.controller.step(
                action="ToggleObjectOn",
                objectId=obj.metadata["id"],
                forceAction=False)
        
        return self.controller.last_event

class make_coffee(toggle_on):
    '''turn on a device'''
    def __init__(self, problem, *obj_args) -> None:
        assert len(obj_args) == 2
        super().__init__(problem, obj_args[0])
    
    def execute(self):
        return super().execute()

class toggle_off(Action):
    '''turn off a device'''
    def __init__(self, problem, *obj_args) -> None:
        assert len(obj_args) == 1
        super().__init__(problem, *obj_args)
    
    def execute(self):
        obj = self.args[0]
        if obj.metadata["isToggled"]:
            event = self.controller.step(
            action="ToggleObjectOff",
            objectId=obj.metadata["id"],
            forceAction=False)
        
        return self.controller.last_event


