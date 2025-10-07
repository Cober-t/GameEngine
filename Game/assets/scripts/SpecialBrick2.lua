
SpecialBrick2
{

	TransformComponent
	{
		position = 1.000000, 1.500000, 0.000000
		rotation = 0.000000, 0.000000, 0.000000
		scale = 1.000000, 1.000000, 1.000000
	}

	Rigidbody2D
	{
		bodyType = 0
		fixedRotation = 0
	}

	BoxCollider2D
	{
		offset = 0.000000, 0.000000
		size = 1.000000, 1.000000
		density = 1.000000
		friction = 0.500000
		restitution = 1.000000
		isSensor = 0
	}

	Render2DComponent
	{
		color = 1.000000, 1.000000, 1.000000, 1.000000
		shape2D = Sprite
		texture = C:\Users\Jorge\Documents\GameEngine\Projects\Breakout\assets\images\blocks.png
		issubtexture = 1.000000

		subtexture
		{
			indices = 5.000000, 10.000000
			cellsize = 32.000000, 16.000000
			spritesize = 1.000000, 1.000000
		}
	}

	NativeScriptComponent
	{
		className = BrickScript
	}
}
