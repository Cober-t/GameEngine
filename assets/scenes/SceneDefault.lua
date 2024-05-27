
SceneDefault
{
	numEntities = 2

	Entity0
	{

		UUID
		{
			id = 17731108172934002688
		}

		TagComponent
		{
			tag = Entity1
		}

		TransformComponent
		{
			position = 0.000000, 2.000000, 0.000000
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
			restitution = 0.999000
		}

		Render2DComponent
		{
			color = 0.934363, 0.281391, 0.000000, 1.000000
			shape2D = Circle
			thickness = 1.000000
			fade = 0.000000
		}
	}

	Entity1
	{

		UUID
		{
			id = 15790550996833163264
		}

		TagComponent
		{
			tag = Entity0
		}

		TransformComponent
		{
			position = 0.000000, -2.729699, 0.000000
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
			restitution = 0.999000
		}

		Render2DComponent
		{
			color = 0.141679, 0.569354, 0.833977, 1.000000
			shape2D = Quad
			fill = 1
		}
	}
}
