
class Snake
{
public:
	bool Move()
	{
	}

	std::vector<IEntity*> m_Body;
};

class Field
{
public:
	Field(uint m_Size);

	uint m_Size;
	
};

class SnakeGame
{
public:
	SnakeGame()
	{
	}
	void Start()
	{
	}
	void Restart()
	{
	}
	void Continue()
	{
	}

	void Update()
	{
	}

private:
	void GenerateBorder()
	{
		for (size_t i = 0; i < m_fi; i++)
		{
		}	
	}

public:
	uint m_FieldSize = 21;

};