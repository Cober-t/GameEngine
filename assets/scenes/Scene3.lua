
Scene3
{
	numEntities = 5

	Entity0
	{

		UUID
		{
			id = 15790550996833163264
		}

		TagComponent
		{
			tag = Entity
		}

		TransformComponent
		{
			position = 0.376314, -3.616013, 0.000000
			rotation = 0.000000, 0.000000, 0.000000
			scale = 7.000000, 3.000000, 1.000000
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
			color = 1.000000, 1.000000, 1.000000, 1.000000
			shape2D = Sprite
			texture = C:\Users\Jorge\Desktop\MVP Engine Test\assets\images\test.jpg
		}
	}

	Entity1
	{

		UUID
		{
			id = 6042340447828528128
		}

		TagComponent
		{
			tag = Entity3
		}

		TransformComponent
		{
			position = 0.147647, 3.016678, 0.000000
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
			color = 1.000000, 1.000000, 1.000000, 1.000000
			shape2D = Circle
			thickness = 1.000000
			fade = 0.005000
		}
	}

	Entity2
	{

		UUID
		{
			id = 10064499163424993280
		}

		TagComponent
		{
			tag = Entity4
		}

		TransformComponent
		{
			position = -0.632452, 2.000000, 0.000000
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
			color = 1.000000, 1.000000, 1.000000, 1.000000
			shape2D = Circle
			thickness = 1.000000
			fade = 0.005000
		}
	}

	Entity3
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
			position = -1.654943, 2.896210, 0.000000
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
			color = 1.000000, 1.000000, 1.000000, 1.000000
			shape2D = Circle
			thickness = 1.000000
			fade = 0.005000
		}
	}

	Entity4
	{

		UUID
		{
			id = 9643654215064920064
		}

		TagComponent
		{
			tag = Entity2
		}

		TransformComponent
		{
			position = -2.062589, 1.384583, 0.000000
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
			color = 1.000000, 1.000000, 1.000000, 1.000000
			shape2D = Circle
			thickness = 1.000000
			fade = 0.005000
		}
	}
}
