import java.util.Scanner;

public class main {

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

    public static byte[] doubleToHex(double number) {
        long bits = Double.doubleToLongBits(number);
        return new byte[] {
            (byte) (bits >> 56),
            (byte) (bits >> 48),
            (byte) (bits >> 40),
            (byte) (bits >> 32),
            (byte) (bits >> 24),
            (byte) (bits >> 16),
            (byte) (bits >> 8),
            (byte) bits
        };
    }

    public static byte[] floatToHex(float number) {
        int bits = Float.floatToIntBits(number);
        return new byte[] {
            (byte) (bits >> 24),
            (byte) (bits >> 16),
            (byte) (bits >> 8),
            (byte) bits
        };
    }

    public static void printHex(byte[] hex) {
        for (byte b : hex) {
            System.out.printf("%02X", b);
        }
        System.out.println();
    }

    public static double hexToDouble(String hex) {
        long bits = 0;
        for (int i = 0; i < hex.length(); i++) {
            char hexChar = hex.charAt(i);
            int hexValue = hexToDecimal(hexChar);
            bits = (bits << 4) | hexValue; // сдвигаем влево и добавляем новую цифру
        }
        return Double.longBitsToDouble(bits);
    }

    public static float hexToFloat(String hex) {
        int bits = 0;
        for (int i = 0; i < hex.length(); i++) {
            char hexChar = hex.charAt(i);
            int hexValue = hexToDecimal(hexChar);
            bits = (bits << 4) | hexValue; // сдвигаем влево и добавляем новую цифру
        }
        return Float.intBitsToFloat(bits);
    }

    private static int hexToDecimal(char hexChar) {
        if (hexChar >= '0' && hexChar <= '9') {
            return hexChar - '0';
        } else if (hexChar >= 'A' && hexChar <= 'F') {
            return hexChar - 'A' + 10;
        } else if (hexChar >= 'a' && hexChar <= 'f') {
            return hexChar - 'a' + 10;
        } else {
            throw new IllegalArgumentException("Недопустимый шестнадцатеричный символ: " + hexChar);
        }
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        int choice;
        boolean loop = true;


        while (loop) {
            System.out.println("Введите 1, если хотите перевечти число в двоично-десятичный код.");
            System.out.println("Введите 2, если хотите декодировать двоично-десятичный код.");
            System.out.println("Введите 3, если хотите преобразовать число с плавающей запятой в шестнадцатиричный код.");
            System.out.println("Введите 4, если хотите декодировать шестнадцатиричный код.");
            System.out.println("Если хотите завершить работу программы, введите 0.");
            choice = scanner.nextInt();
            if (choice == 0) {
                loop = false;
            }else if (choice == 1) {
                System.out.println("Введите целое число для перевода в двоично-десятичный код:");
                int number = scanner.nextInt();
                System.out.println("Двоично-десятичный код: " + toBinaryDecimal(number));
            }else if (choice == 2) {
                System.out.println("Введите двоично-десятичный код для декодирования:");
                String binaryInput = scanner.next();
                try {
                    System.out.println("Декодированное число: " + fromBinaryDecimal(binaryInput));
                } catch (Exception e) {
                    System.out.println("Ошибка: " + e.getMessage());
                }
            }else if (choice == 3) {
                System.out.println("Введите число с плавающей запятой:");
                double number = scanner.nextDouble();

                byte[] eightByteHex = doubleToHex(number);
                byte[] fourByteHex = floatToHex((float) number);

                System.out.println(number);
                System.out.print("Восьмибайтное: ");
                printHex(eightByteHex);
                System.out.print("Четырехбайтное: ");
                printHex(fourByteHex);
                System.out.println();
            }else if (choice == 4) {
                // Ввод восьмибайтного кода
                System.out.println("Введите шестнадцатеричный восьмибайтный код (например, C073600000000000): ");
                String eightByteHex = scanner.next();
                double convertedDouble = hexToDouble(eightByteHex);
                System.out.println("Преобразованное число (double): " + convertedDouble);

                // Ввод четырехбайтного кода
                System.out.print("Введите шестнадцатеричный четырехбайтный код (например, C0AB6000): ");
                String fourByteHex = scanner.next();
                float convertedFloat = hexToFloat(fourByteHex);
                System.out.println("Преобразованное число (float): " + convertedFloat);
            }else System.out.println("Введено некорректное значение.");
        }
        scanner.close();
    }
}
