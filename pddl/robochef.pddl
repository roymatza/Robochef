;Header and description

(define (domain robochef)

;remove requirements that are not needed
(:requirements :adl :strips :fluents :typing :derived-predicates)


;todo: enumerate types and their hierarchy here, e.g. car truck bus - vehicle
(:types
    ; location
    pickupable receptacle surface - object
    cookable tableware cookware - pickupable
    pan pot - cookware 
    fork knife plate mug - tableware
    mug plate togglable cookware - receptacle
    potato egg - cookable
    cookable - food
    table countertop stoveburner - surface
    coffeemachine stoveburner - togglable
)


(:predicates ;todo: define predicates here
    ;(at ?y ?x) ;true iff an object ?y is around an object ?x
    (near ?x) ;true iff the object is by the robot
    (interactable ?x) ;true iff the object can be interacted by the robot (visible, close, unobstructed)
    (held ?x) ;true iff the robot holds ?x
    ; (holding ?r) ;derived from 'is-held'
    (contains ?y - receptacle ?x) ;true iff ?x is contained in ?y
    (has-coffee ?y - receptacle) ;true iff ?x is filled with coffee
    (empty ?y - receptacle) ;true iff the object is free
    ; (occupied ?y - receptacle) ;dervied from 'contains'
    (on ?y - surface ?x - pickupable) ;true iff ?x is on top of ?y
    (cooked ?x - cookable) ;true iff ?x is cooked
    (egg-cracked ?e - egg) ;true iff ?x is cracked
    (toggled ?x - togglable) ;true iff the obect is toggled on
)

; (:functions
;     (heat ?f - food)    
; )

;(:derived (holding) ;true iff the robot is holding something
;    (exists (?x) (is-held ?x ?r))
;)

;(:derived (occupied ?y - receptacle) ;true iff a receptacle is currently used
;    (exists (?x) (contains ?y ?x))
;)

(:derived (empty ?y - receptacle) ;true iff a receptacle is currently used
   (forall (?x) (not(contains ?y ?x)))
)


;define actions here
(:action move-robot
    :parameters (?src ?dest)
    :precondition (and (near ?src) (not (near ?dest)))
    :effect (and (near ?dest) (not (near ?src)) (not(interactable ?src)))
)

(:action make-interactable
    :parameters (?x)
    :precondition (and (near ?x) (not(interactable ?x)) )
    :effect (interactable ?x)
)

(:action pickup
    :parameters (?x - pickupable)
    :precondition (and (interactable ?x) (forall (?z) (not (held ?z))))
    :effect (and (held ?x)
    (forall (?z - surface) (not (on ?z ?x))) 
    (forall (?z - receptacle) (not (contains ?z ?x))))
)

(:action ungrab
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
    :parameters (?x - pickupable ?y - receptacle)
    :precondition (and (held ?x) (interactable ?y) (empty ?y))
    :effect (and (not (held ?x)) (contains ?y ?x))
)

(:action make-coffee
    :parameters (?m - mug ?cm - coffeeMachine)
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
    :parameters (?c - cookable ?p - cookware ?s - stoveburner)
    :precondition (and (toggled ?s) (contains ?s ?p) (contains ?p ?c))
    :effect (cooked ?c)
)

(:action break-egg
    :parameters (?e - egg)
    :precondition (and (not(cooked ?e)) (held ?e) (not (egg-cracked ?e)))
    :effect (egg-cracked ?e)
)

; (:action cook-egg
;     :parameters (?e - egg ?p - cookware)
;     :precondition (and (interactable ?p) (held ?e) (egg-cracked ?e) (empty ?p))
;     :effect (and (cooked ?e) (not (held ?e)) (contains ?p ?e))
; )

)