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
        "put-in-toaster": "place",
        "put-in-cofeemachine": "place",
        "break-egg": "slice",
        "slice-vegetable":"slice",
        "slice-bread":"slice",
        "cook": "cook",
        "toast": "cook",
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
        
    
    def reach(self, obj):
        relevant_poses = self.controller.step(
            action="GetInteractablePoses",
            objectId= obj.metadata["id"],
            rotations=list(range(0, 360, 10)),
            standings=[True]
        ).metadata["actionReturn"]

        #In case the object can be made visible
        if len(relevant_poses) > 0:
            random_pose = random.choice(relevant_poses)
            self.controller.step(
                action="TeleportFull",
                forceAction=True,
                **random_pose
            )
            return True
        else:
            return False

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
            if arg_str == "start_loc":
                action_objects.append(Object(None))
                continue
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
    
    
    def execute(self):
        dest_obj = self.args[0]
        if self.reach(dest_obj):
            return self.controller.last_event
        #In case the object is not reachable
        else:     
            #Find all parent receptacles
            parents = [p_ob for p_ob in self.scene.objects if p_ob.metadata["id"] in dest_obj.metadata["parentReceptacles"]]
            for p in parents:
                #Go near them and try to open them
                if p.metadata["openable"]:
                    no_open = True
                    while self.reach(p) and no_open:
                        no_open = not(self.controller.step(
                            action="OpenObject",
                            objectId=p.metadata["id"],
                            openness=1,
                            forceAction=False
                        ).metadata["lastActionSuccess"])
                #Check if now the required object is reachable
                if self.reach(dest_obj):
                    return self.controller.last_event
            
            #Try to change pose
            for i in range(10):
                if self.reach(dest_obj):
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

        if not self.reach(obj):
            raise Exception(f'object {obj.metadata["name"]} is not reachable')
        
        event = self.controller.step(
            action="PickupObject",
            objectId=obj.metadata["id"],
            forceAction=False,
            manualInteract=False
        )

        return event
    
class place(Action):
    '''place an object onto a surface or in a receptacle'''
    def __init__(self, problem, *obj_args) -> None:
        assert len(obj_args) == 2
        super().__init__(problem, *obj_args)
    
    def execute(self):
        to_place, surface = self.args[0], self.args[1]

        if not self.reach(surface):
            raise Exception(f'object {surface.metadata["name"]} is not reachable')

        event = self.controller.step(
            action="PutObject",
            objectId=surface.metadata["id"],
            forceAction=True,
            placeStationary=True
        )

        #in case the item cannot be placed
        if not event.metadata["lastActionSuccess"]:

            #put the object aside (find a free receptacle) or drop it
            free_receptacles = list(filter(lambda x: x.metadata["receptacle"] and x.metadata["interactable"], self.scene.objects))
            if len(free_receptacles) > 0:
                for rec in free_receptacles:
                    if rec.metadata["openable"]:
                        event = self.controller.step(
                            action="OpenObject",
                            objectId=rec.metadata["id"],
                            openness = 1,
                            forceAction=True
                        )
                    event = self.controller.step(
                        action="PutObject",
                        objectId=rec.metadata["id"],
                        forceAction=True,
                        placeStationary=True
                    )
                    
                    if event.metadata["lastActionSuccess"]:
                        break
                
                if not event.metadata["lastActionSuccess"]:
                    event = self.controller.step(
                        action="DropHandObject",
                        forceAction=True
                    )
            
            #clear the receptacle from other items
            to_clear = filter(lambda x: x.metadata["parentReceptacles"] is not None and surface.metadata["id"] in x.metadata["parentReceptacles"], self.scene.objects)

            for tc in to_clear:
                if self.reach(tc):
                    event = self.controller.step(
                        action="PickupObject",
                        objectId=tc.metadata["id"],
                        forceAction=True,
                        manualInteract=False
                    )
                    event = self.controller.step(
                        action="DropHandObject",
                        forceAction=True
                    )
            
            #try to replace the object
            event = self.controller.step(
                action="PickupObject",
                objectId=to_place.metadata["id"],
                forceAction=True,
                manualInteract=False
            )            
            event = self.controller.step(
                action="PutObject",
                objectId=surface.metadata["id"],
                forceAction=True,
                placeStationary=True
            )
        
        return event

class slice(Action):
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
            name = obj.metadata["name"]
            type = obj.metadata["type"]
            if obj.metadata["type"] == "egg":
                new_id = obj.metadata["id"] + "|EggCracked_0"
            elif obj.metadata["type"] == "bread":
                new_id = obj.metadata["id"] + "|BreadSliced_1"
            else:
                return

            obj_to_update = next(filter(lambda x: x.metadata["name"] == name, self.scene.objects))
            new_metadata = next(filter(lambda x: x["objectId"] == new_id, self.controller.last_event.metadata["objects"]))
            obj_to_update.fromScene(new_metadata)
            obj_to_update.metadata["name"], obj_to_update.metadata["type"], obj_to_update.metadata["id"] = name, type, new_id

        return event

class cook(Action):
    '''cook an object'''
    def __init__(self, problem, *obj_args) -> None:
        assert (len(obj_args) == 3 or len(obj_args) == 2)
        super().__init__(problem, *obj_args)
    
    def execute(self):
        to_cook = self.args[0]
        if not to_cook.metadata["cookable"]:
            raise Exception(f'Object {to_cook.metadata["name"]} is not cookable')

        #Check current state of the object (reserve unique name and type)
        name, type = to_cook.metadata["name"], to_cook.metadata["type"]
        new_metadata = next(filter(lambda x: x["objectId"] == to_cook.metadata["id"], self.controller.last_event.metadata["objects"]))
        to_cook.fromScene(new_metadata)
        to_cook.metadata["name"], to_cook.metadata["type"] = name, type
        
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
                if obj.metadata["id"] in knob.metadata["controlledObjects"]:
                    if not self.reach(knob):
                        raise Exception(f'object {knob.metadata["name"]} is not reachable')
                    
                    self.controller.step(
                    action="ToggleObjectOn",
                    objectId=knob_id,
                    forceAction=False)
                    break
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


