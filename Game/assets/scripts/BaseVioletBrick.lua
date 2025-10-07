
BaseVioletBrick
{

	TransformComponent
	{
		position = -3.000000, 1.500000, 0.000000
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
			indices = 0.000000, 1.000000
			cellsize = 32.000000, 16.000000
			spritesize = 1.000000, 1.000000
		}
	}

	NativeScriptComponent
	{
		className = BrickScript
	}

	ParticleEmitterComponent
	{
		positionVariation = 2.000000, 1.500000
		rotation = 0.000000
		sizeBegin = 0.800000
		sizeEnd = 0.200000
		sizeVariation = 0.000000
		velocity = 0.000000, -0.005000
		velocityVariation = 0.000000, 0.001000
		colorBegin = 1.000000, 1.000000, 1.000000, 1.000000
		colorEnd = 1.000000, 1.000000, 1.000000, 1.000000
		lifeTime = 0.400000
		lifeRemaining = 1.000000
		rate = 8
		active = 1
		loop = 1
		texture = C:\Users\Jorge\Documents\GameEngine\Projects\Breakout\assets\images\particle.png
		issubtexture = 0.000000
	}
}
