package sequential;
import java.util.Scanner;


public class MultiplyCRS {

	public static CRS read_matrix(int dimension, float emptypercent) {
		CRS matrix = new CRS();
		int rowptrval = 0;
		for (int i = 0; i < dimension; i++) {
			matrix.addrow(rowptrval);
			for (int j = 0; j < dimension; j++)
				if (Math.random() > emptypercent) {
					matrix.addelement(j, (float) Math.random());
					rowptrval++;
				}
		}
		matrix.addrow(rowptrval);
		
		return matrix;
		/*
		System.out.print("Rowptr = ");
		for (int i : rowptr)
			System.out.print(i + " ");
		System.out.println();
		System.out.println("colindex = " + colindex);
		System.out.println("val = " + val);
		*/
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		Scanner sc = new Scanner(System.in);
		System.out.println("Dimension: ");
		int dimension = sc.nextInt();
		System.out.println("% Empty elements (0-99):");
		float percentage = ((float) sc.nextInt()) / 100;

		CRS matrixA = read_matrix(dimension, percentage), matrixB = read_matrix(dimension, percentage), product = new CRS();

		int rowptr = 0;
		for (int i = 0; i < matrixA.size(); i++) {
			product.addrow(rowptr);
			for (int j = 0; j < matrixA.size(); j++) {
				float sum = 0;
				for (int k = matrixA.rowptr(i); k < matrixA.rowptr(i + 1); k++) {
					int col = matrixA.getColIndex(k);
					float val1 = matrixA.getVal(k);
					Float val2 = matrixB.getVal(col, j);
					if (val2 != null)
						sum += val1 * val2;
				}
				if (sum != 0) {
					product.addelement(j, sum);
					rowptr++;
				}
			}
		}
		product.addrow(rowptr);
		

		/* To verify */
		float[][] A = matrixA.getMatrixPlain(), B = matrixB.getMatrixPlain(), C = new float[A.length][B[0].length];
		
		for (int i = 0; i < A.length; i++) {
			for (int j = 0; j < A[0].length; j++) {
				float sum = 0;
				for (int k = 0; k < A[0].length; k++)
					sum += A[i][k] * B[k][j];
				C[i][j] = sum;
			}
		}
		
		float[][] result = product.getMatrixPlain();
		for (int i = 0; i < A.length; i++) {
			for (int j = 0; j < A[0].length; j++) {
				if (result[i][j] != C[i][j]) {
					System.err.println("Error! CRS[" + i + "][" + j +     "] = " + result[i][j] + ", plain[" + i + "][" + j + "] = " + C[i][j]);
					System.exit(1);
				}
			}
		}
		

		/*
		System.out.println("A = ");
		matrixA.print();
		System.out.println("B = ");
		matrixB.print();
		System.out.println("Product = ");
		product.print();
		*/
		
		sc.close();
	}

}