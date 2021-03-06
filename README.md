# FlySimulator
Программа, моделирующая поведение мух в замкнутом пространстве.

## Как использовать
- Запустить приложение.
- Настроить "Размер" - поля, "Емкость" – одной ячейки, "Тупость" используя ползунки.
- Нажать кнопку "Запуск" для запуска симуляции. 
- Нажать кнопку "Стоп" чтобы остановить симуляцию, откроется окно статистики.
- Если необходимо еще симулировать перейти к шагу 2, иначе закрыть программу.
> В процессе симуляции используйте кнопку «Добавить» для добавления новых мух на поле. Текущее положение мух на поле отображается количеством живых и мертвых мух в ячейке (красная цифра и серая цифра соответсвенно).

## Как работает
Симулятор состоит из двух частей: 
- Полётное ядро
- Пользовательский интерфейс

### Полётное ядро
**FlySimulator** - Управляет моделью поведения мух. Добавляет новые потоки, создает мух, настраивает свойства модели. Хранит в себе список добавленных мух, список потоков в котором живут мухи и поле. При добавлении мухи, создается поток в который передается муха, алгоритм жизни мухи и поле. Метод stop позволяет немедленно остановить все потоки.
	
**Fly** - Муха, расчитывает движения и телеметрию. Хранит в себе текущие координаты, врожденную тупость и телеметрические данные.
	
**Area** - Поле по которому передвигается муха. Поле состоит из ячеек в котором указано общее количество мух и количество мертвых мух. Ячейки расположены в ассоциативном контейнере, ключ является составное 32-разрядное число из 2 16-разрядных координат X,Y (см. createKey). Передвижение мухи по полю есть по сути изменение числа владельцев ячейки. Когда муха создана она пытается приземлиться с произвольными координатами на поле, если ей это удается в отдельном потоке запускается алгоритм жизни. Вначале муха пытается переместиться по произвольным координатам в соседнюю ячейку (см. goIn), если результат положительный, то она освобождает ранее занимаемую ячейку (см. goOut).  Для атомарного изменения ячеек используется мьютекс. Когда муха умирает вызывается метод die фиксирующий количество мертвых мух в ячейке. Метод population - позволяет получить число всех и мертвых мух.

### Пользовательский интерфейс
Пользовательский интерфейс сделан на Qt, состоит из следующих компонентов

##### Матрица для отображения цикла жизни мух
*MatrixModel* (QAbstractTableModel), *QTableView* и *Renderer* – реализация QT model/view паттерна. 
**MatrixModel** - получает от симулятора данные о популяции мух и отображает в QTableView. Агрегирует в себе поле (Area) и подключается к ней через сигнально-слотовую связь для нотификации об изменении состояния поля.При каждом изменении состояния Area отправляет сигнал updated(). MatrixModel реагирует на это слотом updateAll() и инициирует перерисовку таблицы. В функции data() MatrixModel получает данные из Area и конвертирует в формат подходящий для  Renderer.
**Renderer** - запрашивает из модели популяцию мух в ячейке и отрисовывает количество живых и мертвых мух.

##### Органы управления процессами симуляции и настройки параметров
Стандартные контролы из QT SDK не требуют особых заметок.

##### Окно отображения статистики
Вся статистика собирается в одну строку и отображается в QTextBrowser.


