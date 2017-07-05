/* 文件名 : MammalInt.java */
public class hello implements Animal{
 
   public void eat(){
      System.out.println("Mammal eats");
   }
 
   public void travel(){
      System.out.println("Mammal travels");
   } 
 
   public int noOfLegs(){
      return 0;
   }
 
   public static void main(String args[]){
      hello m = new hello();
      m.eat();
      m.travel();
   }
}
