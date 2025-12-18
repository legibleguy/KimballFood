# KimballFood - a basic layout for handling food items in your Unreal Engine games

i created this for a game with a code name Kimball but it has ended up being abstract enough for me to share it!
This plugin creates a foundation for the food and cooking mechanics in UE5. it heavily relies on the useage of gameplay tags and data assets for defining ingredients and meals

## UKimballFoodIngredient
a data asset class for the ingredient
an ingredient makes up food (aka a whole meal). here is what it consists of
 * a static mesh that visualizes it
 * a gameplay tag
 * (optional) an ingredient positioning class that this particular ingredient might enforce
    for example, a bottom or top bun ingredient will enfore a sandwich positioning class

## UFoodPositioningComponent
this component dictates what ingredient goes after what in the food actor 
for example, there is a built in StackinPositioningComponent in this plugin that ensures we have the bottom bun ingredient at the very bototm of the foor actor, the top bottom at the very top, but then any other ingredients added to the actor are visualized in between those two

## AFoodActor
this actor that brings all the ingredients together. every ingredient is visualized using UInstancedStaticMeshComponent.
you can also set the positioning component to the actor. as mentioned earlier, this will dictate the order and specific transform rules for each ingredient within the actor. once a positioning component is set, we don't allowing resetting it to something else. the reasoning was that you can't just make a burger and somehow turn it into a hotdog by adding the hotdog bun ingredient. at that point we would expect the player to get rid of the burger meal and jsut start a new hotdog meal from scratch

## KimballFoodSettings and KimballFoodDataAsset
the Food settings class allows me to expose settings related to this plugin in the Proejct Settings tab. In the KimballFoodDataAsset will conatin a reference to a specific food actor class you want to use by default in your game. i don't expect you to jsut use the vanilla AFoodActor in the final version of your game so once you do end up making a child implementataion of that class, you may update the reference to it in the project settings so that the rest of your code knows what to work with

## KimballFoodSubsystem
this is a GameInstance subsystem so it will load up everytime your game session starts.
the subsystem loads up the mesh references for each gameplay tag as you define in your data assets. This allows us to quickly load the geometry for new ingredients by using gameplay tags. it does the some for default positioning classes for each ingredient, if you specify any in your data assets for the ingredients
