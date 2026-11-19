#include <stdio.h>
#include <stdlib.h>

int main() {
    system ("chcp 65001 > nul");
    int memory[256];    // массив из 256 ячеек
    int cpu_state[8];   // регистр (быстрые ячейки памяти) процессора
    int i = 0;          // счётчик для перебора ячеек массива
    int pc = 0;         // счётчик команд (хранит адрес следующей команды)
    int operand = 0;    // переменная для хранения аргумента или адреса
    FILE *file = 0;     // для читания program.txt

    while (i < 256)
    {
        memory[i] = 0;
        i = i + 1;
    }

    i = 0;
    while (i < 8)
    {
        cpu_state[i] = 0;
        i = i + 1;
    }

    file = fopen ("program.txt", "r");
    if (file == 0)
    {
        printf ("Ошибка: не удалось открыть файл programm.txt\n");
        return 1;
    }

    i = 0;
    while (i < 256 && fscanf (file, "%d", &memory[i]) == 1) 
    {
        i = i + 1;
    }
    fclose (file);

    cpu_state[0] = 1;
    cpu_state[1] = 0;
    cpu_state[2] = 0;
    cpu_state[3] = memory[0];
    cpu_state[4] = 0;
    cpu_state[5] = 0;

    while (cpu_state[4] == 0)
    {
        pc = cpu_state[0];

        if (pc < 0 || pc >= 256)
        {
            printf ("Ошибка: счётчик вышел за границы памяти\n", pc);
            if (cpu_state[5] != 0 || cpu_state[3] < 0 || cpu_state[3] >= 256)
            {
                cpu_state[4] = 1;
            }
            else
            {
                cpu_state[5] = 1;
                cpu_state[0] = cpu_state[3];
            }
        }
        else
        {
            cpu_state[2] = memory[pc];

            if (cpu_state[2] == 0)      // Команда 0
            {
                printf ("pc=%d: команда 0 (стоп), операнд=нет, acc=%d\n", pc, cpu_state[1]);
                cpu_state[4] = 1;
            }
            else if (cpu_state[2] == 1)     // Команда 1
            {
            cpu_state[1] = cpu_state[1] + 1;
            printf ("pc=%d: команда 1, операнд=нет, acc=%d\n", pc, cpu_state[1]);
            cpu_state[0] = pc + 1;
        }
        else if (cpu_state[2] == 2)     // Команда 2
        {
            cpu_state[1] = cpu_state[1] - 1;
            printf ("pc=%d: команда 2, операнд=нет, acc=%d\n", pc, cpu_state[1]);
            cpu_state[0] = pc + 1;
        }
        else if (cpu_state[2] == 3)     // Команда 3
        {
            if (pc + 1 >= 256)
            {
                printf ("pc=%d: команда 3, ОШИБКА: нет ячейки для операнда\n", pc);
                if (cpu_state[5] != 0 || cpu_state[3] < 0 || cpu_state[3] >= 256)
                {
                    cpu_state[4] = 1;
                }
                else
                {
                    cpu_state[5] = 1;
                    cpu_state[0] = cpu_state[3];
                }
            }
            else
            {
                operand = memory[pc + 1];
                if (operand < 0 || operand >= 256)
                {
                    printf ("pc=%d: команда 3, operand=%d, ОШИБКА: выход за границы памяти\n", pc, operand);
                    if (cpu_state[5] != 0 || cpu_state[3] < 0 || cpu_state[3] >= 256)
                    {
                        cpu_state[4] = 1;
                    }
                    else{
                        cpu_state[5] = 1;
                        cpu_state[0] = cpu_state[3];
                    }
                }
                else
                {
                    cpu_state[1] = cpu_state[1] + memory[operand];
                    printf ("pc=%d: команда 3, operand=%d, acc=%d\n", pc, operand, cpu_state[1]);
                    cpu_state[0] = pc + 2;
                }
            }
            }
            else
            {
                printf ("pc=%d: неизвестная команда %d, ОШИБКА\n", pc, cpu_state[2]);
                if (cpu_state[5] != 0 || cpu_state[3] < 0 || cpu_state[3] >= 256)
                {
                    cpu_state[4] = 1;
                }
                else{
                    cpu_state[5] = 2;
                    cpu_state[0] = cpu_state[3];
                }
            }
        }

        if (cpu_state[5] != 0)
        {
            printf ("Программа завершена с ошибкой %d.\n", cpu_state[5]);
        }
        else{
            printf ("Программа успешно завершена. Итоговый acc = %d\n", cpu_state[1]);
        }
        
        return 0;

    }

}