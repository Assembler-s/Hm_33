#include <iostream>
#include <string>
#include <windows.h>

using namespace std;

// ---------------------------------------------------------------
// Potion — стан зілля в котлі
// ---------------------------------------------------------------
class Potion
{
private:
	int healing;   // лікувальна сила
	int toxicity;  // токсичність

public:
	Potion() : healing(0), toxicity(0) {}

	void addHealing(int amount)
	{
		healing += amount;
	}

	void addToxicity(int amount)
	{
		toxicity += amount;
	}

	void dilute()
	{
		toxicity /= 2;
	}

	void amplify()
	{
		healing *= 2;
		toxicity *= 2;
	}

	// Рівень 3
	void transmute()
	{
		healing += toxicity;
		toxicity = 0;
	}

	string getResult() const
	{
		if (healing + toxicity > 200)
		{
			return "Котел вибухнув!";
		}

		if (healing == 0 && toxicity == 0)
		{
			return "Брудна вода";
		}

		if (toxicity > healing)
		{
			return "Отрута";
		}

		return "Зілля лікування";
	}

	void print() const
	{
		cout << "Лікування: " << healing << ", токсичність: " << toxicity
			<< " → " << getResult() << endl;
	}
};

// ---------------------------------------------------------------
// Ingredient — базовий клас, інертна домішка
// ---------------------------------------------------------------
class Ingredient
{
protected:
	string name;
	int power;

public:
	Ingredient(const string& name, int power) : name(name), power(power) {}

	virtual ~Ingredient() = default;

	string getName() const
	{
		return name;
	}

	virtual void addTo(Potion& potion) const
	{
		cout << name << " розчиняється без сліду" << endl;
	}
};

// ---------------------------------------------------------------
// Нащадки
// ---------------------------------------------------------------
class Herb : public Ingredient
{
public:
	Herb(const string& name, int power) : Ingredient(name, power) {}

	void addTo(Potion& potion) const override
	{
		potion.addHealing(power);
		cout << name << ": +" << power << " до лікування" << endl;
	}
};

class Mushroom : public Ingredient
{
public:
	Mushroom(const string& name, int power) : Ingredient(name, power) {}

	void addTo(Potion& potion) const override
	{
		potion.addToxicity(power);
		cout << name << ": +" << power << " до токсичності" << endl;
	}
};

class Water : public Ingredient
{
public:
	Water() : Ingredient("Чиста вода", 0) {}

	void addTo(Potion& potion) const override
	{
		potion.dilute();
		cout << name << ": токсичність зменшено вдвічі" << endl;
	}
};

class Crystal : public Ingredient
{
public:
	Crystal(const string& name) : Ingredient(name, 0) {}

	void addTo(Potion& potion) const override
	{
		potion.amplify();
		cout << name << ": усе в котлі посилено вдвічі" << endl;
	}
};

// Рівень 3: єдиний у своєму роді, тому final
class PhilosopherStone final : public Ingredient
{
public:
	PhilosopherStone() : Ingredient("Філософський камінь", 0) {}

	void addTo(Potion& potion) const override
	{
		potion.transmute();
		cout << name << ": отруту перетворено на ліки" << endl;
	}
};

// ---------------------------------------------------------------
// throwIn — кинути інгредієнт у зілля
// ---------------------------------------------------------------
void throwIn(Potion& potion, const Ingredient& ingredient)
{
	cout << "Кидаємо: " << ingredient.getName() << endl;
	ingredient.addTo(potion);
}

// ---------------------------------------------------------------
// Рівень 2: Cauldron — котел володіє інгредієнтами
// ---------------------------------------------------------------
class Cauldron
{
private:
	Ingredient** ingredients;  // динамічний масив покажчиків
	int count;

public:
	Cauldron() : ingredients(nullptr), count(0) {}

	Cauldron(const Cauldron&) = delete;
	Cauldron& operator=(const Cauldron&) = delete;

	~Cauldron()
	{
		clear();
	}

	bool add(Ingredient* ingredient)
	{
		if (ingredient == nullptr)
		{
			return false;
		}

		Ingredient** newArray = new Ingredient * [count + 1];

		for (int i = 0; i < count; i++)
		{
			newArray[i] = ingredients[i];
		}

		newArray[count] = ingredient;

		delete[] ingredients;
		ingredients = newArray;
		count++;

		return true;
	}

	void clear()
	{
		for (int i = 0; i < count; i++)
		{
			delete ingredients[i];
		}

		delete[] ingredients;
		ingredients = nullptr;
		count = 0;
	}

	void brew()
	{
		Potion potion;

		for (int i = 0; i < count; i++)
		{
			ingredients[i]->addTo(potion);
		}

		potion.print();
		clear();
	}
};

int main()
{
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);

	// ----------------------- Рівень 1 -----------------------
	cout << "===== Рівень 1 =====" << endl;

	Herb mint("М'ята", 30);
	Mushroom fly("Мухомор", 40);
	Crystal moonCrystal("Місячний кристал");
	Ingredient sand("Пісок", 0);
	Water water;

	cout << "\n--- Зілля 1: м'ята, кристал, мухомор ---" << endl;
	Potion potion1;
	throwIn(potion1, mint);
	throwIn(potion1, moonCrystal);
	throwIn(potion1, fly);
	potion1.print();

	cout << "\n--- Зілля 2: мухомор, кристал, м'ята ---" << endl;
	Potion potion2;
	throwIn(potion2, fly);
	throwIn(potion2, moonCrystal);
	throwIn(potion2, mint);
	potion2.print();

	cout << "\n--- Зілля 3: мухомор, пісок, вода ---" << endl;
	Potion potion3;
	throwIn(potion3, fly);
	throwIn(potion3, sand);
	throwIn(potion3, water);
	potion3.print();

	// ----------------------- Рівень 2 -----------------------
	cout << "\n===== Рівень 2 =====" << endl;

	Cauldron cauldron;

	cout << "\n--- Рецепт 1 ---" << endl;
	cauldron.add(new Herb("Женьшень", 50));
	cauldron.add(new Herb("М'ята", 30));
	cauldron.add(new Crystal("Місячний кристал"));
	cauldron.add(new Mushroom("Поганка", 20));
	cauldron.add(new Water());
	cauldron.brew();

	cout << "\n--- Рецепт 2 ---" << endl;
	cauldron.add(new Herb("Женьшень", 50));
	cauldron.add(new Crystal("Місячний кристал"));
	cauldron.add(new Mushroom("Мухомор", 40));
	cauldron.add(new Crystal("Місячний кристал"));
	cauldron.brew();

	// ----------------------- Рівень 3 -----------------------
	cout << "\n===== Рівень 3 =====" << endl;

	cauldron.add(new Mushroom("Мухомор", 40));
	cauldron.add(new Mushroom("Поганка", 20));
	cauldron.add(new PhilosopherStone());
	cauldron.brew();

	return 0;
}

/*
1) Через виртуальную функцию addTo. Вызов идёт через ссылку или указатель на Ingredient, но выбирается версия по реальному
типу объекта (позднее связывание, через vtable). throwIn и котёл конкретных типов не знают.

2) getName() одинаков для всех наследников и нигде не переопределяется, поэтому динамический вызов ему не нужен. Если
убрать virtual в addTo, а вместе с ним и override (с override без virtual в базе код не скомпилируется), вызов через const
Ingredient& будет выбирать Ingredient::addTo по статическому типу. Все инградиенты станут инертными («розчиняється без
сліду»), и все зелья выйдут «Брудна вода» (0 / 0).

3) Сигнатура без const не совпадает с базовой, это другая функция.
а) С override: ошибка компиляции, метод ничего не переопределяет.
б) Без override: компилируется, но это новый метод, который скрывает базовый. Через Ingredient& или Ingredient*
вызовется инертная базовая версия, и мухомор ничего не добавит. Ошибку молча проглатывает компилятор, поэтому override и нужен.

4) Срезка (slicing). Копия создаётся как чистый Ingredient: производная часть отбрасывается, addTo вызывается базовая,
мухомор превращается в песок. Плюс лишнее копирование. Если сделать Ingredient абстрактным (вопрос 7),
такая сигнатура вообще не скомпилируется.

5) Из-за delete через указатель на базовый класс. Без виртуального деструктора это неопределённое поведение: вызовется
только ~Ingredient, а деструктор наследника нет. Здесь у наследников нет ресурсов, и ничего не сломается, но формально
это UB. Это происходит в Cauldron::clear(): delete ingredients[i]. clear() вызывается из brew() и из ~Cauldron().

6) 0 строк. Котёл хранит Ingredient* и вызывает виртуальный addTo, поэтому новый наследник подключается без правок в
котле и в throwIn. Изменились только Potion (добавили transmute) и список классов (добавили PhilosopherStone).

7) Нет, не имеет. «Просто Ingredient» ничего не делает и в котле бесполезен. Чтобы запретить такие объекты, нужно сделать
addTo чисто виртуальным: virtual void addTo(Potion&) const = 0;. Тогда Ingredient становится абстрактным, и Ingredient
sand(...) не скомпилируется. Песок придётся вынести в отдельный наследник, например Sand, с пустым addTo.
Второй вариант, слабее: сделать конструктор Ingredient защищённым (protected).
*/