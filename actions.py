from copy import Error
from scene_info import Scene
from scene_info import Object
import random

class Action:
    #names of corresponding actions in pddl
    actionsDict = {
        "make-interactable": "make_interactable",
        "move-robot": "move_robot",
        "place": "place",
        "put": "place",
        "break": "break_object",
        "break-egg": "break_object",
        "cook": "cook",
        "turn-on": "toggle_on",
        "make-coffee": "toggle_on",
        "turn-off": "toggle_off"
    }
    
    def __init__(self, scene: Scene, *obj_args) -> None:
        assert (all([isinstance(arg,Object) for arg in obj_args]) and len(obj_args) > 0) 
        self.scene = scene
        self.args = obj_args
        
    
    def execute(self):
        pass

    @staticmethod
    def parse(action_str, objectsDic):
        action_words = action_str.split()
        
        action_name = action_words[0]
        len_args = len(action_words) - 1

        action_objects = [objectsDic[arg_str] for arg_str in action_words[1:len_args+1]]
        action = globals()[Action.actionsDict[action_name]]

        return action(*action_objects) 

class make_interactable(Action):
    '''change robot from one and pose to another and do some actions in order to make an object visible'''
    def __init__(self, scene, *obj_args) -> None:
        assert len(obj_args) == 1
        super().__init__(scene, *obj_args)
    
    def __reach(self, obj):
        relevant_poses = self.scene.controller.step(
            action="GetInteractablePoses",
            objectId= obj.metadata["id"],
            standings=[True]
        ).metadata["actionReturn"]

        #In case the object can be made visible
        if len(relevant_poses) > 0:
            random_pose = random.choice(relevant_poses)
            self.scene.controller.step(
                action="TeleportFull",
                **random_pose
            )
            return True
        else:
            return False
    
    def execute(self):
        dest_obj = self.args[0]
        if self.__reach(dest_obj):
            return self.scene.controller.step(action="Done")
        #In case the object is not reachable
        else:     
            #Find all parent receptacles
            parents = [p_ob for p_ob in self.scene.objects if p_ob.metadata["id"] in dest_obj.metadata["parentReceptacles"]]
            for p in parents:
                #Go near them and try to open them
                if p.metadata["openable"]:
                    if self.__reach(p):
                        self.scene.controller.step(
                            action="OpenObject",
                            objectId=p.metadata["id"],
                            openness=1,
                            forceAction=False
                        )
                #Check if now the required object is reachable
                if self.__reach(dest_obj):
                    return self.scene.controller.step(action="Done")
            
            #Try to change pose
            for i in range(10):
                if self.__reach(dest_obj):
                    return self.scene.controller.step(action="Done")


        raise Exception(f'object {dest_obj.metadata["name"]} is not reachable')

class move_robot(Action):
    '''move robot towards an object'''
    def __init__(self, scene, *obj_args) -> None:
        assert len(obj_args) == 2
        super().__init__(scene, obj_args)
    
    def execute(self):
        pass

class pickup(Action):
    '''pickup an object that is interactable to the robot'''
    def __init__(self, scene, *obj_args) -> None:
        assert len(obj_args) == 1
        super().__init__(scene, obj_args)
    
    def execute(self):
        obj = self.args[0]
        if not obj.metadata["pickupable"]:
            raise Exception(f'object {obj.metadata["name"]} is not pickupable')


        event = self.scene.controller.step(
            action="PickupObject",
            objectId=obj.metadata["id"],
            forceAction=False,
            manualInteract=False
        )

        return event
    
class place(Action):
    '''place an object onto a surface'''
    def __init__(self, scene, *obj_args) -> None:
        assert len(obj_args) == 2
        super().__init__(scene, obj_args)
    
    def execute(self):
        to_place, surface = self.args[0], self.args[1]

        if not to_place.metadata["isPickedUp"]:
            Exception(f'object {to_place.metadata["name"]} is not picked up')
        if not surface.metadata["interactable"]:
            Exception(f'object {surface.metadata["name"]} is not interactable')
            
            # Exception(f'object {to_place.metadata["name"]} can\'t be placed on {surface.metadata["name"]}')

        event = self.scene.controller.step(
            action="PutObject",
            objectId=to_place.metadata["id"],
            forceAction=False,
            placeStationary=True
        )
        
        return event

class break_object(Action):
    '''move robot towards an object'''
    def __init__(self, scene, *obj_args) -> None:
        assert len(obj_args) == 1
        super().__init__(scene, obj_args)
    
    def execute(self):
        obj = self.args[0]
        if not obj.metadata["breakable"]:
            raise Exception(f'object {obj.metadata["name"]} is not breakable')

        event = self.scene.controller.step(
            action="BreakObject",
            objectId=obj.metadata["id"],
            forceAction=False
        )

        return event

class cook(Action):
    '''cook an object'''
    def __init__(self, scene, *obj_args) -> None:
        assert len(obj_args) == 3
        super().__init__(scene, obj_args)
    
    def execute(self):
        to_cook = self.args[0]
        if not to_cook.metadata["isCookable"]:
            raise Exception(f'Object {to_cook.metadata["name"]} is not cookable')

        if not to_cook.metadata["isCooked"]:
            event = self.scene.controller.step(
                action="CookObject",
                objectId=to_cook.metadata["id"],
                forceAction=False)
        
        return event
        
class toggle_on(Action):
    '''turn on a device'''
    def __init__(self, scene, *obj_args) -> None:
        assert len(obj_args) == 1
        super().__init__(scene, obj_args)
    
    def execute(self):
        obj = self.args[0]
        if not obj.metadata["isToggled"]:
            event = self.scene.controller.step(
            action="ToggleObjectOn",
            objectId=obj.metadata["id"],
            forceAction=False)

class toggle_off(Action):
    '''turn off a device'''
    def __init__(self, scene, *obj_args) -> None:
        assert len(obj_args) == 1
        super().__init__(scene, obj_args)
    
    def execute(self):
        obj = self.args[0]
        if obj.metadata["isToggled"]:
            event = self.scene.controller.step(
            action="ToggleObjectOff",
            objectId=obj.metadata["id"],
            forceAction=False)


