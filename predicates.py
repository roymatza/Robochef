from scene_info import Object
from scene_info import visibilityDistance

class Predicate:
    def __init__(self, name, *obj_args) -> None:
        assert (all([isinstance(arg,Object) for arg in obj_args]) and len(obj_args) > 0)
        self.name = name
        self.args = obj_args
        
    
    def value(self):
        pass
    
    def unparse(self):
        s = "{} {}"
        pred_name = self.name
        arg_names = [arg.metadata["name"] for arg in self.args]
        return s.format(self.name, ' '.join(arg_names))

    @staticmethod
    def parse(pred_str, objectsDic):
        pred_words = pred_str.split()
        
        #Skip negation 
        if pred_words[0] == 'not':
            pred_words.pop(0)
        
        pred_name = pred_words[0]
        len_args = len(pred_words) - 1

        pred_objects = [objectsDic[arg_str] for arg_str in pred_words[1:len_args+1]]
        predicate = globals()[pred_name]

        return predicate(*pred_objects)    

class on(Predicate):
    def __init__(self, *obj_args) -> None:
        assert len(obj_args) == 2
        super().__init__("on", *obj_args)
    
    def value(self):
        obj1, obj2 = self.args[0], self.args[1]
        return ((obj2.metadata["receptacleObjectIds"] is not None) and 
        (obj1.metadata["id"] in obj2.metadata["receptacleObjectIds"]))

class contains(Predicate):
    def __init__(self, *obj_args) -> None:
        assert len(obj_args) == 2
        super().__init__("contains", *obj_args)
    
    def value(self):
        obj1, obj2 = self.args[0], self.args[1]
        return ((obj2.metadata["receptacleObjectIds"] is not None) and 
        (obj1.metadata["id"] in obj2.metadata["receptacleObjectIds"]))

class empty(Predicate):
    def __init__(self, *obj_args) -> None:
        assert len(obj_args) == 1
        super().__init__("empty", *obj_args)

    def value(self):
        obj1 = self.args[0]
        return (len(obj1.metadata["receptacleObjectIds"]) == 0)
        
class cooked(Predicate):
    def __init__(self, *obj_args) -> None:
        assert len(obj_args) == 1
        super().__init__("cooked", *obj_args)
    
    def value(self):
        obj1 = self.args[0]
        return (obj1.metadata["isCooked"])

class has_coffee(Predicate):
    def __init__(self, *obj_args) -> None:
        assert len(obj_args) == 1
        super().__init__("has-coffee", *obj_args)
    
    def value(self):
        obj1 = self.args[0]
        return (obj1.metadata["isFilledWithLiquid"] and obj1.metadata["fillLiquid"] == "coffee")
        
class interactable(Predicate):
    def __init__(self, *obj_args) -> None:
        assert len(obj_args) == 1
        super().__init__("interactable", *obj_args)
    
    def value(self):
        obj1 = self.args[0]
        return (obj1.metadata["interactable"])

class held(Predicate):
    def __init__(self, *obj_args) -> None:
        assert len(obj_args) == 1
        super().__init__("held", *obj_args)
    
    def value(self):
        obj1 = self.args[0]
        return (obj1.metadata["isPickedUp"])

class near(Predicate):
    def __init__(self, *obj_args) -> None:
        assert len(obj_args) == 1
        super().__init__("near", *obj_args)
    
    def value(self):
        obj1 = self.args[0]
        return (obj1.metadata["distance"] <= visibilityDistance)

class toggled(Predicate):
    def __init__(self, *obj_args) -> None:
        assert len(obj_args) == 1
        super().__init__("toggled", *obj_args)
    
    def value(self):
        obj1 = self.args[0]
        return (obj1.metadata["isToggled"])

class egg_cracked(Predicate):
    def __init__(self, *obj_args) -> None:
        assert len(obj_args) == 1
        super().__init__("egg-cracked", *obj_args)
    
    def value(self):
        obj1 = self.args[0]
        return (obj1.metadata["type"] == "EggCracked")  
