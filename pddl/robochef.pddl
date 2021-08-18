;Header and description

(define (domain robochef)

;remove requirements that are not needed
(:requirements :adl :strips :fluents :typing)


;todo: enumerate types and their hierarchy here, e.g. car truck bus - vehicle
(:types
    ; location
    robot - object
    cookable receptacle - locatable
    food liquid - cookable
    mug pan plate coffeeMachine - receptacle
    egg - food
    coffee water - liquid
    table countertop - surface 
)

(:constants
    SELF - robot
)

(:predicates ;todo: define predicates here
    (is-at ?y ?x) ;true iff an object ?y is around an object ?x
    (is-visible ?x - object ?r - robot) ;true iff the object visible by the robot
    (is-held ?x - object ?r - robot) ;true iff the robot holds ?x
    ; (holding ?r) ;derived from 'is-held'
    (contains ?y - receptacle ?x) ;true iff ?x is contained in ?y
    ; (occupied ?y - receptacle) ;dervied from 'contains'
    (on ?y - surface ?x - locatable) ;true iff ?x is on top of ?y
    (is-cooked ?x - cookable) ;true iff ?x is cooked
    (is-egg-cracked ?e - egg) ;true iff ?x is cracked
    (is-coffeMachine-available ?cm - coffeeMachine) ;true iff the coffee machine is free use
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
    :parameters (?src ?dest)
    :precondition (and (is-at SELF ?src) (not (is-at SELF ?dest)))
    :effect (and (not (is-at SELF ?src)) (is-at SELF ?dest))
)

(:action grab
    :parameters (?x - locatable)
    :precondition (and (is-visible ?x SELF) (is-at SELF ?x) (forall (?z) (not (is-held ?z SELF))))
    :effect (is-held ?x SELF)
)

(:action ungrab
    :parameters (?x - locatable)
    :precondition (is-held ?x SELF)
    :effect (not (is-held ?x SELF)) 
)

(:action place
    :parameters (?x - locatable ?y - surface)
    :precondition (and (is-held ?x SELF) (is-at SELF ?y))
    :effect (and (not (is-held ?x SELF)) (on ?y ?x))
)

(:action fill
    :parameters (?x - locatable ?y - receptacle)
    :precondition (and (is-held ?x SELF) (is-at SELF ?y))
    :effect (and (not (is-held ?x SELF)) (contains ?y ?x))
)

(:action make-coffee
    :parameters (?c - coffee ?m - mug ?cm - coffeeMachine)
    :precondition (and (is-visible ?cm SELF) (is-coffeMachine-available ?cm) (contains ?cm ?m))
    :effect (and (not (is-coffeMachine-available ?cm)) (contains ?m ?c))
)

(:action break-egg
    :parameters (?e - egg)
    :precondition (and (is-held ?e SELF) (not (is-egg-cracked ?e)))
    :effect (is-egg-cracked ?e)
)

(:action cook-egg
    :parameters (?e - egg ?p - pan)
    :precondition (and (is-visible ?p SELF) (is-egg-cracked ?e) (is-held ?e SELF) (forall (?z) (not (contains ?p ?z))))
    :effect (and (is-cooked ?e) (not (is-held ?e SELF)) (contains ?p ?e))
)

)