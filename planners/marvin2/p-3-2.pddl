(define (problem GL-3-2)
(:domain GROCERYLIST)
(:objects H SONE STWO GONE GTWO GTHREE)
(:INIT (isHome H) (isShop SONE) (isShop STWO) (isProduct GONE) (isProduct GTWO) (isProduct GTHREE)
       (AgentLocation H) (ProductShop SONE GONE) (ProductShop SONE GTWO) (ProductShop STWO GTHREE))
(:goal (AND (AgentLocation H) (bought GONE) (bought GTWO) (bought GTHREE)))
)

