from ai2thor.controller import Controller
import copy

visibilityDistance = 1 #Visibility distance of agent (m)

class TypeNameHandler:
    def __init__(self) -> None:
        self.typingDict = dict()
    
    def generate_name(self, typeName: str) -> None:
        if typeName in self.typingDict.keys():
            self.typingDict[typeName]["n"] += 1
        else:
            self.typingDict[typeName] = {"n" : 1}

        return str(str.lower(typeName)+str(self.typingDict[typeName]["n"]))

class Object:

    typeNameHandler = TypeNameHandler()

    def __init__(self,fields: dict) -> None:
        if fields is not None:
            self.metadata = fields
        else:
            self.metadata = dict()

    def update(self, fields: dict):
        self.metadata.update(fields)
    
    def fromScene(self, objMetadata):
        if objMetadata is not None:
            
            #Take a copy of the original metadata
            metadata = copy.deepcopy(objMetadata)

            #Update some fields for compatability 
            metadata.update({
            'name': self.typeNameHandler.generate_name(objMetadata["objectType"]),
            'id': objMetadata["objectId"],
            'type': str.lower(objMetadata["objectType"]),
            'interactable': objMetadata["visible"], #assumed to be interactable when seen
            'position': (objMetadata["position"]["x"], objMetadata["position"]["y"], objMetadata["position"]["z"]),
            'temperature': Scene.temperatureNumericValue(objMetadata["temperature"]),
            })

            #Clean some unwanted fields
            metadata.pop("objectType", None)
            metadata.pop("objectTemperature", None)
            metadata.pop("objectId", None)
        else:
            metadata = dict()

        self.metadata = metadata
        

class Scene:
    '''Fetches important information about the scene in terms of pddl model'''

    def __init__(self, controller: Controller) -> None:
        '''Gets an event object of the current scene'''
        self.controller = controller
        self.objects, self.visibleObjects, self.holding = self.getObjectsState(self.controller)
    
    def getObjectsState(self, controller):
        '''Returns a dictionary of all objects in the scene, their types and other properties'''

        event = controller.step(action="Done")

        listObj = []
        listVisible = []
        heldObject = None
        
        for obj in event.metadata["objects"]:
            currObj = Object(fields=None)
            currObj.fromScene(obj)
            listObj.append(currObj)

            if currObj.metadata["isPickedUp"]:
                heldObject = currObj.metadata["name"]
        
            if currObj.metadata["visible"]:
                listVisible.append(currObj.metadata["name"])
            
        return listObj, listVisible, heldObject

    @staticmethod
    def temperatureNumericValue(tmp_str):
        if tmp_str == 'Cold':
            return 0
        if tmp_str == 'RoomTemp':
            return 1
        if tmp_str == 'Hot':
            return 2
    

    # def checkObjectReachability(self, obj):
    #     corners = obj["objectOrientedBoundingBox"]
    #     return ((corners in self.reachablePositions) and obj["visible"])

    



    

        

