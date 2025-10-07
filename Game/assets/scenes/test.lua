
test
{

	Settings
	{
		SceneType = 2D
		Gravity = -9.800000
		Perspective = 1
	}
	numEntities = 12

	Entity0
	{

		UUID
		{
			id = 7394625923261237248
		}

		TagComponent
		{
			tag = PerspTopLeft
		}

		TransformComponent
		{
			position = -10.000000, 5.000000, 0.000000
			rotation = 0.000000, 0.000000, 0.000000
			scale = 0.250000, 0.250000, 1.000000
		}

		Render2DComponent
		{
			color = 0.957529, 0.375498, 0.096123, 1.000000
			shape2D = Quad
			fill = 1
		}
	}

	Entity1
	{

		UUID
		{
			id = 3319563783829660672
		}

		TagComponent
		{
			tag = OrthoBottom
		}

		TransformComponent
		{
			position = 0.000000, -5.000000, 0.000000
			rotation = 0.000000, 0.000000, 0.000000
			scale = 0.250000, 0.250000, 1.000000
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
			color = 0.956863, 0.376471, 0.098039, 1.000000
			shape2D = Quad
			fill = 1
		}
	}

	Entity2
	{

		UUID
		{
			id = 2023442292035184128
		}

		TagComponent
		{
			tag = Camera
		}

		TransformComponent
		{
			position = 0.000000, 0.000000, 0.000000
			rotation = 0.000000, 0.000000, 0.000000
			scale = 1.000000, 1.000000, 1.000000
		}

		CameraComponent
		{
			distance = 5.000000
			width = 1291
			height = 720
			nearClip = 0.010000
			farClip = 1000.000000
			fov = 45.000000
			perspective = 1
			mainCamera = 0
			debug = 1
		}
	}

	Entity3
	{

		UUID
		{
			id = 13359810411513438208
		}

		TagComponent
		{
			tag = OrthoBottomLeft
		}

		TransformComponent
		{
			position = -10.000000, -5.000000, 0.000000
			rotation = 0.000000, 0.000000, 0.000000
			scale = 0.250000, 0.250000, 1.000000
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
			color = 0.956863, 0.376471, 0.098039, 1.000000
			shape2D = Quad
			fill = 1
		}
	}

	Entity4
	{

		UUID
		{
			id = 6711192834977143808
		}

		TagComponent
		{
			tag = PerspTopRight
		}

		TransformComponent
		{
			position = 0.292796, 3.495056, 0.000000
			rotation = 0.000000, 0.000000, 0.000000
			scale = 0.250000, 0.250000, 1.000000
		}

		Rigidbody2D
		{
			bodyType = 2
			fixedRotation = 0
		}

		BoxCollider2D
		{
			offset = 0.000000, 0.000000
			size = 1.000000, 1.000000
			density = 0.130000
			friction = 0.500000
			restitution = 0.000000
			isSensor = 0
		}

		Render2DComponent
		{
			color = 0.957529, 0.375498, 0.096123, 1.000000
			shape2D = Quad
			fill = 1
		}

		ParticleEmitterComponent
		{
			positionVariation = 0.000000, 0.000000
			rotation = 0.000000
			sizeBegin = 7.850000
			sizeEnd = 3.210000
			sizeVariation = 0.000000
			velocity = 0.000000, 0.000000
			velocityVariation = 1.400000, -0.400000
			colorBegin = 1.000000, 1.000000, 1.000000, 1.000000
			colorEnd = 1.000000, 1.000000, 1.000000, 1.000000
			lifeTime = 1.000000
			lifeRemaining = 0.000000
			rate = 1
			active = 1
			loop = 1
			texture = C:\Users\Jorge\Documents\GameEngine\Game\assets\images\hearts.png
			issubtexture = 0.000000
		}
	}

	Entity5
	{

		UUID
		{
			id = 8317721661667212288
		}

		TagComponent
		{
			tag = PerspBottomLeft
		}

		TransformComponent
		{
			position = -3.700000, -2.000000, 0.000000
			rotation = 0.000000, 0.000000, 0.000000
			scale = 0.250000, 0.250000, 1.000000
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
			color = 0.956863, 0.376471, 0.098039, 1.000000
			shape2D = Quad
			fill = 1
		}
	}

	Entity6
	{

		UUID
		{
			id = 14311360269555308544
		}

		TagComponent
		{
			tag = PerspBottomRight
		}

		TransformComponent
		{
			position = 3.700000, -2.000000, 0.000000
			rotation = 0.000000, 0.000000, 0.000000
			scale = 0.250000, 0.250000, 1.000000
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
			color = 0.956863, 0.376471, 0.098039, 1.000000
			shape2D = Quad
			fill = 1
		}
	}

	Entity7
	{

		UUID
		{
			id = 5519493176052812800
		}

		TagComponent
		{
			tag = OrthoTop
		}

		TransformComponent
		{
			position = 0.000000, 5.000000, 0.000000
			rotation = 0.000000, 0.000000, 0.000000
			scale = 0.250000, 0.250000, 1.000000
		}

		Render2DComponent
		{
			color = 0.957529, 0.375498, 0.096123, 1.000000
			shape2D = Quad
			fill = 1
		}
	}

	Entity8
	{

		UUID
		{
			id = 11289082211900358656
		}

		TagComponent
		{
			tag = OrthoTopLeft
		}

		TransformComponent
		{
			position = -3.700000, 2.000000, 0.000000
			rotation = 0.000000, 0.000000, 0.000000
			scale = 0.250000, 0.250000, 1.000000
		}

		Render2DComponent
		{
			color = 0.957529, 0.375498, 0.096123, 1.000000
			shape2D = Quad
			fill = 1
		}
	}

	Entity9
	{

		UUID
		{
			id = 15768207574558115840
		}

		TagComponent
		{
			tag = SpriteTest
		}

		TransformComponent
		{
			position = 0.000000, 0.000000, 0.000000
			rotation = 0.000000, 0.000000, 0.000000
			scale = 5.000000, 5.000000, 1.000000
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
			texture = C:\Users\Jorge\Documents\GameEngine\Game\assets\images\arrows.png
			issubtexture = 0.000000
		}

		NativeScriptComponent
		{
			className = TestScript
		}
	}

	Entity10
	{

		UUID
		{
			id = 13365495593844561920
		}

		TagComponent
		{
			tag = OrthoTopRight
		}

		TransformComponent
		{
			position = 10.000000, 5.000000, 0.000000
			rotation = 0.000000, 0.000000, 0.000000
			scale = 0.250000, 0.250000, 1.000000
		}

		Render2DComponent
		{
			color = 0.957529, 0.375498, 0.096123, 1.000000
			shape2D = Quad
			fill = 1
		}
	}

	Entity11
	{

		UUID
		{
			id = 11101716906685786112
		}

		TagComponent
		{
			tag = OrthoBottomRight
		}

		TransformComponent
		{
			position = 10.000000, -5.000000, 0.000000
			rotation = 0.000000, 0.000000, 0.000000
			scale = 0.250000, 0.250000, 1.000000
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
			color = 0.956863, 0.376471, 0.098039, 1.000000
			shape2D = Quad
			fill = 1
		}
	}
}
