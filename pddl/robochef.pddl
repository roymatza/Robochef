;Header and description

(define (domain robochef)

;remove requirements that are not needed
(:requirements :adl :strips :fluents :typing :derived-predicates)


;todo: enumerate types and their hierarchy here, e.g. car truck bus - vehicle
(:types
    pickupable receptacle surface - object
    tableware cookware sliceable - pickupable
    cookable vegetable - sliceable
    lettuce tomato - vegetable
    bowl spoon fork knife plate mug cup - tableware
    generic_receptacle togglable - receptacle
    bowl sink mug plate cookware - generic_receptacle
    pan pot - cookware
    cookable-in-cookware bread - cookable
    potato egg - cookable-in-cookware
    table countertop stoveburner - surface
    toaster coffeemachine stoveburner - togglable
)

(:constants
    START_LOC ;a dummy constant for the robot starting point
)


(:predicates ;todo: define predicates here
    (near ?x) ;true iff the object is by the robot
    (interactable ?x) ;true iff the object can be interacted by the robot (visible, close, unobstructed)
    (held ?x - pickupable) ;true iff the robot holds ?x
    (holding) ;derived from 'is-held'
    (contains ?y - receptacle ?x) ;true iff ?x is contained in ?y
    (has-coffee ?y - receptacle) ;true iff ?x is filled with coffee
    (empty ?y - receptacle) ;true iff the object is free
    (on ?y - surface ?x - pickupable) ;true iff ?x is on top of ?y
    (cooked ?x - cookable) ;true iff ?x is cooked
    (egg-cracked ?e - egg) ;true iff ?x is cracked
    (toggled ?x - togglable) ;true iff the obect is toggled on
    (sliced ?x - sliceable) ;true iff the object is sliceable
)

(:derived (holding) ;true iff the robot is holding something
   (exists (?x - pickupable) (held ?x))
)

(:derived (empty ?y - receptacle) ;true iff a receptacle is currently used
   (forall (?x) (not(contains ?y ?x)))
)

;define actions here
(:action move-robot
    :parameters (?src ?dest)
    :precondition (and (near ?src) (not (near ?dest)))
    :effect (and (near ?dest)
    (not (near ?src))
    (forall (?z) (not(interactable ?z))))
    ;(forall (?z) (when (not (= ?z ?src)) (not(interactable ?z)))))
)

(:action make-interactable
    :parameters (?x)
    :precondition (and (near ?x) (not(interactable ?x)))
    :effect (interactable ?x)
)

(:action pickup
    :parameters (?x - pickupable)
    :precondition (and (interactable ?x) (forall (?z - pickupable) (not (held ?z))))
    :effect (and (held ?x)
    (forall (?z - surface) (not (on ?z ?x))) 
    (forall (?z - receptacle) (not (contains ?z ?x))))
)

(:action drop
    :parameters (?x - pickupable)
    :precondition (held ?x)
    :effect (not (held ?x))
)

(:action place
    :parameters (?x - pickupable ?y - surface)
    :precondition (and (held ?x) (interactable ?y))
    :effect (and (not (held ?x)) (on ?y ?x))
)

(:action put
    :parameters (?x - pickupable ?y - generic_receptacle)
    :precondition (and (held ?x) (interactable ?y))
    :effect (and (not (held ?x)) (contains ?y ?x))
)

(:action put-in-cofeemachine
    :parameters (?m - mug ?cm - coffeemachine)
    :precondition (and (held ?m) (empty ?m) (interactable ?cm))
    :effect (and (not (held ?m)) (contains ?cm ?m))
)

(:action put-in-toaster
    :parameters (?b - bread ?t - toaster)
    :precondition (and (held ?b) (sliced ?b) (interactable ?t))
    :effect (and (not (held ?b)) (contains ?t ?b))
)

(:action make-coffee
    :parameters (?cm - coffeemachine ?m - mug)
    :precondition (and (interactable ?cm) (not(toggled ?cm)) (contains ?cm ?m))
    :effect (and (has-coffee ?m) (toggled ?cm))
)

(:action turn-on
    :parameters (?x - togglable)
    :precondition (and (not(toggled ?x)) (interactable ?x))
    :effect (toggled ?x)
)

(:action turn-off
    :parameters (?x - togglable)
    :precondition (and (toggled ?x) (interactable ?x))
    :effect (not(toggled ?x))
)

(:action cook
    :parameters (?c - cookable-in-cookware ?p - cookware ?s - stoveburner)
    :precondition (and (toggled ?s) (on ?s ?p) (contains ?p ?c))
    :effect (cooked ?c)
)

(:action toast
    :parameters (?b - bread ?t - toaster)
    :precondition (and (toggled ?t) (contains ?t ?b))
    :effect (cooked ?b)
)

(:action break-egg
    :parameters (?e - egg ?p - pan)
    :precondition (and (interactable ?e) (contains ?p ?e) (not(cooked ?e)) (not (held ?p)) (not (egg-cracked ?e)))
    :effect (egg-cracked ?e)
)

(:action slice-vegetable
    :parameters (?x - vegetable ?b -bowl)
    :precondition (and (not (sliced ?x)) (interactable ?x) (contains ?b ?x))
    :effect (sliced ?x)
)

(:action slice-bread
    :parameters (?x - bread)
    :precondition (and (not (sliced ?x)) (interactable ?x) (not(held ?x)))
    :effect (sliced ?x)
)

)