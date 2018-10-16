
Документация:

Engine:
    JsonLoader:
	Object:
	 Clashes:
	  +-Опередлить глубину проникновения aabb персонажа с aabb объекта уровня(Этот способ рабочий, но имеет кучу недостатков)
	  -Добавить k-dops для динамических и статических объектов 
	  -Опередлить глубину проникновения k-dops персонажа с k-dops объекта уровня(Этот способ рабочий, но имеет кучу недостатков)
	  -Вытолкнуть персонажа 
	DebugWindow:
	 -Использовть lua для создания подобных окон;
	 Интерфейс для посмотра и изменения объектов; Должен представлять из себя один единственный класс, который будет рисовать окно, в котором будут находится все инстанциированные объекты;Должна быть возможность изменять поля класса (скорость игрока или предметы в его инвентаре)
```c++
    class DebugWindow
    {
    	var size 
    	var spawnPos
    	var isSelected
    	var isClose
    	var windowName
    	void makeWindow(std::string name);
    	void addText(std::string text);
    	void makeContextItem(var name, float item, var min, var max, var step); 
    	void addTreeNode(var name, var lambda); //?
    	void makeChildWindow(var name);
    public:
    	void draw(); //or Show
    	template<class T>
    	void setObjectInfo();

    	DebugWindow();
    	~DebugWindow();
    };

```
    Animation manager:
      -Заменить list<string,animation> на list<string,animation&&>(на 10мс быстрее пушит анимацию)
      +загрузка анимаций из json
      +загрузка анимаций из xml
    Debug:
     +-Придумать универсальный интерфейс для вывода значения переменных объекта(позиция/скорость и т.д)
	Level:
	  -Сделать загрузку и отрисовку нескольких тайлсетов 
	  +-Оптимизировать рисование уровня:
	   +Отрисовка статичных объектов в текстуру 
	   -Отрисовка только тех объектов, которые попадают в камеру 
	Debug:
	  +Устранить утечку памяти(Возможно как-то связано с ImGui)
	  +Сделать для объектов(в данном случае игрока)менюшку в которой можно будет изменять состояния объекта(задавать ему импульс, менять угол поворота и т.д)
	  +Сделать обновления localRectangle у object т.к во время анимации widht и height изменяются, и дебажный прямоугольник рисуется 'неправильно'
	  +Переделать функцию IMGUI::objectStatusInfo();(избавиться от tuple и template, передавать значения как массив ссылок? Принимать только целые значения?)
	  +Окно логов: 
	   +Категории логов (info,err,system)
	   +Время логов
	   +Сортировка логов или же поиск
	   +Приврепить окно логов к верхнему участку экрана(w = 1920, h~=400)
	   +Сделать для фиксированну позицию и transparency = 40%
	   +Выводить логи при помощи imGui::text, дабы можно было задавать тексту цвет 
       -Оптимизировать отрисовку rectangles' статических объектов
      -Вызывать отрисовку интерфейса в одном месте(Например DebuggingSystem::drawInterface(...)) 
    Game:
     +Вызов ImGui::Update/Render и т.д в независимости от текущего state(play или ui)
     Time:
      ?Стоит запустить update time в отдельном потоке
    World:
     -World::draw(sf::renderWindow&) - должен вызывать только window->draw()
	GUI:
    +Сделать возможным вызов GUI::draw без RenderWinow::display && clear
	Menu:
	 -Сделать класс более гибким(Отрисовка ui по шаблонам?)
	 Standard template: 
      +-MainUI
	  -PauseUI
      -InventoriUI
    LUA(использовать lua в качестве скриптового языка):
     -InitScript для глобальных данных(созданных в C++)
     +Include LuaBridge 
     +Отрисовка окна ImGui;
      +Вывод текста(Из c++ и Lua)
    Audio:
     Фоновая Музыка:
      -Музыка в меню
      -Музыка в игре
      -Плеер
     Звуки:
      -Звук Ходьбы
      -Звуки нажатия кнопок
      -Звук открытия меню/паузы