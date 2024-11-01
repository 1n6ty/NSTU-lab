import java.util.Scanner;

public class Main {

    // 1) Перевести заданное число в двоично-десятичный код
    public static String toBinaryDecimal(int number) {
        StringBuilder bcd = new StringBuilder();
        while (number > 0) {
            int digit = number % 10;
            bcd.insert(0, String.format("%04d", Integer.parseInt(Integer.toBinaryString(digit))));
            number /= 10;
        }
        return bcd.toString();
    }

    // 2) Расшифровать число из заданного двоично-десятичного кода
    private static int fromBinaryDecimal(String bcd) {
        int decimal = 0;
        for (int i = 0; i < bcd.length(); i += 4) {
            if (i + 4 > bcd.length()) {
                throw new IllegalArgumentException("Недостаточно бит для последней цифры BCD");
            }
            String digitBinary = bcd.substring(i, i + 4);
            int digitDecimal = Integer.parseInt(digitBinary, 2);
            decimal = decimal * 10 + digitDecimal;
        }
        return decimal;
    }

    // Преобразование числа с плавающей запятой в восьмибайтное представление
    public static String doubleToHex(double number) {
        long bits = 0;
        long sign = (number < 0) ? 1 : 0;
        if (number < 0) number = -number;

        int exponent = 0;
        while (number >= 2.0) {
            number /= 2.0;
            exponent++;
        }
        while (number < 1.0) {
            number *= 2.0;
            exponent--;
        }
        
        exponent += 1023;

        // Получение мантиссы
        number -= 1.0;
        long mantissa = 0;
        for (int i = 0; i < 52; i++) {
            number *= 2;
            if (number >= 1.0) {
                mantissa |= 1L << (51 - i);
                number -= 1.0;
            }
        }

        // Формируем биты
        bits |= (sign << 63);
        bits |= ((long) exponent << 52);
        bits |= mantissa;

        return String.format("%016X", bits);
    }

    // Преобразование числа с плавающей запятой в четырёхбайтное представление
    public static String floatToHex(float number) {
        int bits = 0;
        int sign = (number < 0) ? 1 : 0;
        if (number < 0) number = -number;

        int exponent = 0;
        while (number >= 2.0) {
            number /= 2.0;
            exponent++;
        }
        while (number < 1.0) {
            number *= 2.0;
            exponent--;
        }

        exponent += 127;

        // Получение мантиссы
        number -= 1.0;
        int mantissa = 0;
        for (int i = 0; i < 23; i++) {
            number *= 2;
            if (number >= 1.0) {
                mantissa |= 1 << (22 - i);
                number -= 1.0;
            }
        }

        // Формируем биты
        bits |= (sign << 31);
        bits |= (exponent << 23);
        bits |= mantissa;

        return String.format("%08X", bits);
    }

    // Декодирование шестнадцатеричного представления в число double
    public static double hexToDouble(String hex) {
        long bits = Long.parseLong(hex, 16);
        long sign = (bits >> 63) & 1;
        int exponent = (int)((bits >> 52) & 0x7FF);
        long mantissa = bits & 0xFFFFFFFFFFFFFL;

        if (exponent == 0 && mantissa == 0) return 0.0; // 0.0
        if (exponent == 0x7FF) return (mantissa == 0) ? (sign == 0 ? Double.POSITIVE_INFINITY : Double.NEGATIVE_INFINITY) : Double.NaN; // inf/nan

        exponent -= 1023;
        double value = 1.0;
        for (int i = 51; i >= 0; i--) {
            if ((mantissa & (1L << i)) != 0) {
                value += Math.pow(2, i - 52);
            }
        }
        value *= Math.pow(2, exponent);
        return (sign == 0) ? value : -value;
    }

    // Декодирование шестнадцатеричного представления в число float
    public static float hexToFloat(String hex) {
        int bits = Integer.parseInt(hex, 16);
        int sign = (bits >> 31) & 1;
        int exponent = (bits >> 23) & 0xFF;
        int mantissa = bits & 0x7FFFFF;

        if (exponent == 0 && mantissa == 0) return 0.0f; // 0.0
        if (exponent == 0xFF) return (mantissa == 0) ? (sign == 0 ? Float.POSITIVE_INFINITY : Float.NEGATIVE_INFINITY) : Float.NaN; // inf/nan

        exponent -= 127;
        float value = 1.0f;
        for (int i = 22; i >= 0; i--) {
            if ((mantissa & (1 << i)) != 0) {
                value += Math.pow(2, i - 23);
            }
        }
        value *= Math.pow(2, exponent);
        return (sign == 0) ? value : -value;
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        int choice;
        boolean loop = true;

        while (loop) {
            System.out.println("Введите 1, если хотите перевести число в двоично-десятичный код.");
            System.out.println("Введите 2, если хотите декодировать двоично-десятичный код.");
            System.out.println("Введите 3, если хотите преобразовать число с плавающей запятой в шестнадцатеричное восьмибайтное представление (double).");
            System.out.println("Введите 4, если хотите преобразовать число с плавающей запятой в шестнадцатеричное четырёхбайтное представление (float).");
            System.out.println("Введите 5, если хотите декодировать шестнадцатеричный восьмибайтный код.");
            System.out.println("Введите 6, если хотите декодировать шестнадцатеричный четырехбайтный код.");
            System.out.println("Если хотите завершить работу программы, введите 0.");
            choice = scanner.nextInt();
            if (choice == 0) {
                loop = false;
            } else if (choice == 1) {
                System.out.println("Введите целое число для перевода в двоично-десятичный код:");
                int number = scanner.nextInt();
                System.out.println("Двоично-десятичный код: " + toBinaryDecimal(number));
            } else if (choice == 2) {
                System.out.println("Введите двоично-десятичный код для декодирования:");
                String binaryInput = scanner.next();
                System.out.println("Декодированное число: " + fromBinaryDecimal(binaryInput));
            } else if (choice == 3) {
                System.out.println("Введите число с плавающей запятой (double):");
                double number = scanner.nextDouble();
                String hexRepresentation = doubleToHex(number);
                System.out.println("Шестнадцатеричное восьмибайтное представление: " + hexRepresentation);
            } else if (choice == 4) {
                System.out.println("Введите число с плавающей запятой (float):");
                float number = scanner.nextFloat();
                String hexRepresentation = floatToHex(number);
                System.out.println("Шестнадцатеричное четырёхбайтное представление: " + hexRepresentation);
            } else if (choice == 5) {
                System.out.println("Введите шестнадцатеричный восьмибайтный код (например, C073600000000000): ");
                String eightByteHex = scanner.next();
                double convertedDouble = hexToDouble(eightByteHex);
                System.out.println("Преобразованное число (double): " + convertedDouble);
            } else if (choice == 6) {
                System.out.print("Введите шестнадцатеричный четырехбайтный код (например, C0AB6000): ");
                String fourByteHex = scanner.next();
                float convertedFloat = hexToFloat(fourByteHex);
                System.out.println("Преобразованное число (float): " + convertedFloat);
            } else {
                System.out.println("Введено некорректное значение.");
            }
        }
        scanner.close();
    }
}
