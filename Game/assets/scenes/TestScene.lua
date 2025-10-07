
TestScene
{

	Settings
	{
		SceneType = 2D
		Gravity = -9.000000
	}
	numEntities = 2

	Entity0
	{

		UUID
		{
			id = 6711192834977143808
		}

		TagComponent
		{
			tag = Entity0
		}

		TransformComponent
		{
			position = 0.000000, 1.817523, 0.000000
			rotation = 0.000000, 0.000000, 0.000000
			scale = 1.000000, 1.000000, 1.000000
		}

		Rigidbody2D
		{
			bodyType = 2
			fixedRotation = 0
		}

		CircleCollider2D
		{
			offset = 0.000000, 0.000000
			radius = 0.500000
			density = 1.000000
			friction = 0.500000
			restitution = 0.710000
			isSensor = 0
		}

		Render2DComponent
		{
			color = 1.000000, 1.000000, 1.000000, 1.000000
			shape2D = Circle
			thickness = 1.000000
			fade = 0.005000
		}
	}

	Entity1
	{

		UUID
		{
			id = 13359810411513438208
		}

		TagComponent
		{
			tag = Entity
		}

		TransformComponent
		{
			position = 0.000000, -1.746460, 0.000000
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
			restitution = 0.000000
			isSensor = 0
		}

		Render2DComponent
		{
			color = 1.000000, 1.000000, 1.000000, 1.000000
			shape2D = Quad
			fill = 1
		}
	}
}
