[![Build Status](https://travis-ci.org/artemuch12/Point_to_point.svg?branch=master)](https://travis-ci.org/artemuch12/Point_to_point)
# Point_to_point v1.1
# 1. Запуск программы
# Для работы необходимо, чтобы первым запущен сервер, а затем клиент. При запуске клиента, так и сервера необходимо вести IPv4 адрес сервера (это сделано для переносимости программы). В случае если адресс не был введен, то будет установлен адрес 127.0.0.1 - приминяемый для отладки.
# Пример запуска программы
#	user15$ ./server 192.168.1.2
#	user27$ ./client 192.168.1.2

# 2. Работа с программой
# После запуска клиента, появляется функционал ввести сообщение которое будет отправленно серверу. Аналогичный функционал присутсвует и на серверной части. Длина сообщения ограничена 255 символами. Сообщение большего объема будет обрезано до 254 символа.
# После того как диалог был осуществлен, одна из сторон может отключиться, для этого необходимо вести команду "!exit!". После ввода этой команды программа на стороне инициализировавшей разрыв происходит закрытие программы. Другой стороне отправляется уведомление об закрытии программы, после чего программа на стороне принявшей данное сообщение закрывается.

# 3. Изменения в версиях
# 1.0 - Первая версия
# 1.1 - Изменения в работе технического канала сервера и клиента.

# 4. Известные баги и не доработки.
# 1 - При стандартном выходе на строне сервера, клиент не закрывается.
# 2 - При аварийном выходе на стороне клиента, сервер не закрывается.
