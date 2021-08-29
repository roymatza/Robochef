;Header and description

(define (domain robochef_with_self)

;remove requirements that are not needed
(:requirements :adl :strips :fluents :typing)


;todo: enumerate types and their hierarchy here, e.g. car truck bus - vehicle
(:types
    ; location
    robot - object
    pickupable receptacle surface - env_object
    cookable tableware - pickupable
    food liquid - cookable
    mug pan plate coffeeMachine - receptacle
    fork knife plate mug - tableware
    egg - food
    coffee water - liquid
    table countertop - surface 
)

(:constants
    SELF - robot
)

(:predicates ;todo: define predicates here
    ;(at ?y ?x) ;true iff an object ?y is around an object ?x
    (near ?x - env_object ?r - robot) ;true iff the object is by the robot
    (interactable ?x - env_object ?r - robot) ;true iff the object can be interacted by the robot (visible, close, unobstructed)
    (held ?x - object ?r - robot) ;true iff the robot holds ?x
    ; (holding ?r) ;derived from 'is-held'
    (contains ?y - receptacle ?x) ;true iff ?x is contained in ?y
    ; (occupied ?y - receptacle) ;dervied from 'contains'
    (on ?y - surface ?x - pickupable) ;true iff ?x is on top of ?y
    (cooked ?x - cookable) ;true iff ?x is cooked
    (egg-cracked ?e - egg) ;true iff ?x is cracked
    (available ?x) ;true iff the object is free to use
)

; (:functions
;     (heat ?f - food)    
; )

;(:derived (holding ?r - SELF) ;true iff the robot is holding something
;    (exists (?x) (is-held ?x ?r))
;)

;(:derived (occupied ?y - receptacle) ;true iff a receptacle is currently used
;    (exists (?x) (contains ?y ?x))
;)


;define actions here
(:action teleport-robot
    :parameters (?src - env_object ?dest - env_object)
    :precondition (and (near ?src SELF) (not (near ?dest SELF)))
    :effect (and (near ?dest SELF) (not (near ?src SELF)))
)

(:action make-interactable
    :parameters (?x - env_object)
    :precondition (and (near ?x SELF) (not(interactable ?x SELF)) )
    :effect (interactable ?x SELF)
)

(:action grab
    :parameters (?x - pickupable)
    :precondition (and (interactable ?x SELF) (forall (?z) (not (held ?z SELF))))
    :effect (held ?x SELF)
)

(:action ungrab
    :parameters (?x - pickupable)
    :precondition (held ?x SELF)
    :effect (not (held ?x SELF)) 
)

(:action place
    :parameters (?x - pickupable ?y - surface)
    :precondition (and (held ?x SELF) (interactable ?y SELF))
    :effect (and (not (held ?x SELF)) (on ?y ?x))
)

(:action fill
    :parameters (?x - pickupable ?y - receptacle)
    :precondition (and (held ?x SELF) (interactable ?y SELF))
    :effect (and (not (held ?x SELF)) (contains ?y ?x))
)

(:action make-coffee
    :parameters (?c - coffee ?m - mug ?cm - coffeeMachine)
    :precondition (and (interactable ?cm SELF) (available ?cm) (contains ?cm ?m))
    :effect (and (not (available ?cm)) (contains ?m ?c))
)

(:action break-egg
    :parameters (?e - egg)
    :precondition (and (held ?e SELF) (not (egg-cracked ?e)))
    :effect (egg-cracked ?e)
)

(:action cook-egg
    :parameters (?e - egg ?p - pan)
    :precondition (and (interactable ?p SELF) (egg-cracked ?e) (held ?e SELF) (forall (?z) (not (contains ?p ?z))))
    :effect (and (cooked ?e) (not (held ?e SELF)) (contains ?p ?e))
)

)