import java.util.Scanner;


public class main{
    // Метод для перевода из десятичной системы в r-ичную
    public static String decimalToRBase(double decimal, int base) {

        StringBuilder integerPart = new StringBuilder();
        StringBuilder fractionalPart = new StringBuilder();

        // Обработка целой части
        long integer = (long) decimal;
        do {
            int remainder = (int) (integer % base);
            integerPart.insert(0, Character.forDigit(remainder, base));
            integer /= base;
        } while (integer > 0);

        // Обработка дробной части
        double fractional = decimal - (long) decimal;
        while (fractional > 0) {
            fractional *= base;
            int digit = (int) fractional;
            fractionalPart.append(Character.forDigit(digit, base));
            fractional -= digit;
        }

        return integerPart.toString() + (fractionalPart.length() > 0 ? "." + fractionalPart.toString() : "");
    }

    // Метод для перевода из r-ичной системы в десятичную
    public static double rBaseToDecimal(String rBase, int base) {

        String[] parts = rBase.split("\\.");
        double result = 0;

        // Обработка целой части
        String integerPart = parts[0];
        for (int i = 0; i < integerPart.length(); i++) {
            char digit = integerPart.charAt(integerPart.length() - 1 - i);
            int digitValue = Character.digit(digit, base);
            result += digitValue * Math.pow(base, i);
        }

        // Обработка дробной части
        if (parts.length > 1) {
            String fractionalPart = parts[1];
            for (int i = 0; i < fractionalPart.length(); i++) {
                char digit = fractionalPart.charAt(i);
                int digitValue = Character.digit(digit, base);
                result += digitValue * Math.pow(base, -1 * (i + 1));
            }
        }
        return result;
    }

    // Метод для сложения двух r-ичных вещественных чисел
    public static String addRBaseNumbers(String num1, String num2, int base) {
        int p1;
        int p2;
        int p3;
        String res = "";
        String sign1 = num1.startsWith("-") ? "-" : "";
        String sign2 = num2.startsWith("-") ? "-" : "";


        num1 = num1.replace("-", "");
        num2 = num2.replace("-", "");

        p1 = num1.indexOf(".");
        p2 = num2.indexOf(".");

        if (p1 == -1 && p2 == -1) {
            while (num1.length() < num2.length()){
                num1 = "0" + num1;
                p1++;
            } 
            while (num2.length() < num1.length()){
                num2 = "0" + num2;
                p2++;
            }

            if (num1.compareTo(num2) < 0) {
                String v = num1;
                num1 = num2;
                num2 = v;
    
                v = sign1;
                sign1 = sign2;
                sign2 = sign1; 
            }
    
            if (sign1 == "-" && sign2 == "-"){
                res = addRBaseInteger(num1, num2, base);
                res = new StringBuilder(res).toString();
                res = "-" + res;
            }else if(sign1 == "-" && sign2 == ""){
                res = subtractRBaseInteger(num1, num2, base);
                res = new StringBuilder(res).toString();
                res = "-" + res;
            }else if (sign1 == "" && sign2 == "-") {
                res = subtractRBaseInteger(num1, num2, base);
                res = new StringBuilder(res).toString();
            }else if (sign1 == "" && sign2 == ""){
                res = addRBaseInteger(num1, num2, base);
                res = new StringBuilder(res).toString();
            }
        }else{

            while (p1 < p2){
                num1 = "0" + num1;
                p1++;
            } 
            while (p2 < p1){
                num2 = "0" + num2;
                p2++;
            }

            if (p1 > p2) {
                p3 = p1;
            }else p3 = p2;

            p1 = num1.length() - p1 - 1;
            p2 = num2.length() - p2 - 1;
    
            while (p1 < p2){
                num1 = num1 + "0";
                p1++;
            } 
            while (p2 < p1){
                num2 = num2 + "0";
                p2++;
            }
        
            num1 = num1.replace(".", "");
            num2 = num2.replace(".", ""); 
            
            if (num1.compareTo(num2) < 0) {
                String v = num1;
                num1 = num2;
                num2 = v;
    
                v = sign1;
                sign1 = sign2;
                sign2 = sign1; 
            }
    
            if (sign1 == "-" && sign2 == "-"){
                res = addRBaseInteger(num1, num2, base);
                res = new StringBuilder(res).insert(p3-1, ".").toString();
                res = "-" + res;
            }else if(sign1 == "-" && sign2 == ""){
                res = subtractRBaseInteger(num1, num2, base);
                res = new StringBuilder(res).insert(p3-1, ".").toString();
                res = "-" + res;
            }else if (sign1 == "" && sign2 == "-") {
                res = subtractRBaseInteger(num1, num2, base);
                res = new StringBuilder(res).insert(p3-1, ".").toString();
            }else if (sign1 == "" && sign2 == ""){
                res = addRBaseInteger(num1, num2, base);
                res = new StringBuilder(res).insert(p3-1, ".").toString();
            }
        }
       
        return res.replaceFirst("^0+(?!$)", "").replaceFirst("\\.$", ""); // Удаляем ведущие нули и одиночную точку в конце
    }
    
    // Метод для сложения двух r-ичных целых чисел
    private static String addRBaseInteger(String num1, String num2, int base) {
        StringBuilder result = new StringBuilder();
        int carry = 0;
    
        for (int i = num1.length() - 1; i >= 0; i--) {
            int digit1 = Character.digit(num1.charAt(i), base);
            int digit2 = Character.digit(num2.charAt(i), base);
            int sum = digit1 + digit2 + carry;
    
            carry = sum / base;
            result.append(Character.forDigit(sum % base, base));
        }
    
        if (carry > 0) {
            result.append(Character.forDigit(carry, base));
        }
    
        return result.reverse().toString();
    }
    

    private static String subtractRBaseInteger(String num1, String num2, int base) {
        StringBuilder result = new StringBuilder();
        int borrow = 0;
    
        for (int i = num1.length() - 1; i >= 0; i--) {
            int digit1 = Character.digit(num1.charAt(i), base);
            int digit2 = Character.digit(num2.charAt(i), base) + borrow;
    
            if (digit1 < digit2) {
                digit1 += base;
                borrow = 1;
            } else {
                borrow = 0;
            }
    
            result.append(Character.forDigit(digit1 - digit2, base));
        }
    
        // Удаляем ведущие нули
        String finalResult = result.reverse().toString().replaceFirst("^0+(?!$)", "");
        return finalResult.isEmpty() ? "0" : finalResult;
    }

    // Метод для умножения двух r-ичных вещественных чисел
    public static String multiplyRBaseNumbers(String num1, String num2, int base) {
        String sign1 = num1.startsWith("-") ? "-" : "";
        String sign2 = num2.startsWith("-") ? "-" : "";
    
        // Удаляем знаки для упрощения работы
        num1 = num1.replace("-", "");
        num2 = num2.replace("-", "");
    
        int decimalPos1 = num1.indexOf(".");
        int decimalPos2 = num2.indexOf(".");
    
        // Если дробные части отсутствуют, устанавливаем позиции в конец строки
        if (decimalPos1 == -1) decimalPos1 = num1.length();
        if (decimalPos2 == -1) decimalPos2 = num2.length();
    
        // Определяем количество знаков после запятой
        int fractionalDigits1 = num1.length() - decimalPos1 - 1;
        int fractionalDigits2 = num2.length() - decimalPos2 - 1;
    
        // Убираем точки для умножения
        num1 = num1.replace(".", "");
        num2 = num2.replace(".", "");
    
        // Выполняем умножение целых частей
        String result = multiplyRBaseInteger(num1, num2, base);
    
        // Общее количество знаков после запятой в результате
        int totalFractionalDigits = fractionalDigits1 + fractionalDigits2;
        
        // Вставляем точку в нужное место
        if (totalFractionalDigits > 0) {
            if (result.length() <= totalFractionalDigits) {
                result = "0." + "0".repeat(totalFractionalDigits - result.length()) + result;
            } else {
                result = new StringBuilder(result)
                        .insert(result.length() - totalFractionalDigits, ".")
                        .toString();
            }
        }
        
        // Определяем знак результата
        if ((sign1.equals("-") && sign2.equals("")) || (sign1.equals("") && sign2.equals("-"))) {
            result = "-" + result;
        } else if (sign1.equals("-") && sign2.equals("-")) {
            // Знак получается положительным
        }
    
        // Удаляем ведущие нули и точку в конце
        return result.replaceFirst("^0+(?!$)", "").replaceFirst("\\.$", ""); 
    }
    
    
    // Метод для умножения двух r-ичных целых чисел
    private static String multiplyRBaseInteger(String num1, String num2, int base) {
        StringBuilder result = new StringBuilder();
        int[] intermediateResults = new int[num1.length() + num2.length()];

        for (int i = num2.length() - 1; i >= 0; i--) {
            int digit2 = Character.digit(num2.charAt(i), base);
            int carry = 0;

            for (int j = num1.length() - 1; j >= 0; j--) {
                int digit1 = Character.digit(num1.charAt(j), base);
                int product = digit1 * digit2 + carry + intermediateResults[i + j + 1];
                
                carry = product / base;
                intermediateResults[i + j + 1] = product % base;
            }
            intermediateResults[i] += carry;
        }
        for (int num : intermediateResults) {
            if (!(result.length() == 0 && num == 0)) {
                result.append(Character.forDigit(num, base));
            }
        }

        return result.length() == 0 ? "0" : result.toString();
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        boolean way = true;
        int choice;
        int base;

        while (way) {
            System.out.println("To convert a real number from decimal to r-base, enter 1.\n"+
                               "To convert a real number from r-base to 10-base, enter 2.\n"+
                               "To calculate the sum of two real numbers in r-base, enter 3.\n"+
                               "To calculate the product of two real numbers in r-base, enter 4.n"+
                               "For exit enter 0.");
            choice = scanner.nextInt();

            if (choice == 0) {
                way = false;
            }
            
            if (choice == 1) {
                System.out.print("Enter number in 10-base: ");
                double decimalNumber = scanner.nextDouble();
                System.out.print("Enter base culculation system (from 2 to 36): ");
                base = scanner.nextInt();
                
                String rBaseNumber = decimalToRBase(decimalNumber, base);
                System.out.println("Number in " + base + "-base: " + rBaseNumber);                
            }else if (choice == 2){
                System.out.print("Enter base culculation system (from 2 to 36): ");
                base = scanner.nextInt();

                System.out.print("Enter number in " + base + "-base: ");
                String inputRBaseNumber = scanner.next();
                double convertedBack = rBaseToDecimal(inputRBaseNumber, base);
                System.out.println("Number in 10-base: " + convertedBack);
            }else if (choice == 3) {
                System.out.print("Enter base r (from 2 to 36): ");
                base = scanner.nextInt();
        
                System.out.print("Enter first r-base number: ");
                String num1 = scanner.next();
        
                System.out.print("Enter second r-base number: ");
                String num2 = scanner.next();
        
                String sum = addRBaseNumbers(num1, num2, base);
                System.out.println("Sum: " + sum);                
            }else if (choice == 4) {
                System.out.print("Enter base r (from 2 to 36): ");
                base = scanner.nextInt();
        
                System.out.print("Enter first r-base number: ");
                String num1 = scanner.next();
        
                System.out.print("Enter second r-base number: ");
                String num2 = scanner.next();
             
                String product = multiplyRBaseNumbers(num1, num2, base);
                System.out.println("Product: " + product);
            }
        }
        scanner.close();
    }
}