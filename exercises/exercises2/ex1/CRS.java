package sequential;
import java.util.ArrayList;
import java.util.List;


public class CRS {

	private List<Float> val;
	private List<Integer> row_ptr, col_ind;

	public CRS() {
		/*
		this.col_ind = new int[col_ind.length];
		this.val = new float[val.length];
		this.row_ptr = new int[row_ptr.length];
		System.arraycopy(row_ptr, 0, this.row_ptr, 0, this.row_ptr.length);
		System.arraycopy(col_ind, 0, this.col_ind, 0, this.col_ind.length);
		System.arraycopy(val, 0, this.val, 0, this.val.length);
		*/
		this.val = new ArrayList<>();
		col_ind = new ArrayList<>();
		row_ptr = new ArrayList<>();

	}
	
	public int size() {
		return row_ptr.size() - 1;
	}

	public int rowptr(int i) {
		return this.row_ptr.get(i);
	}

	public Float getVal(int row, int col) {
		for (int i = this.row_ptr.get(row); i < this.row_ptr.get(row + 1); i ++)
			if (this.col_ind.get(i) == col)
				return this.val.get(i);
		return null;
	}
	
	public int getColIndex(int k) {
		return this.col_ind.get(k);
	}

	public float getVal(int k) {
		return this.val.get(k);
	}

	public void addrow(int rowptrval) {
		this.row_ptr.add(rowptrval);
	}

	public void addelement(int j, float v) {
		this.col_ind.add(j);
		this.val.add(v);
	}

	public void print() {
		System.out.println("Rowptr = " + row_ptr);
		System.out.println("colindex = " + col_ind);
		System.out.println("val = " + val);

	}

	public float[][] getMatrixPlain() {
		int dim = this.row_ptr.size() - 1;
		float[][] matrix = new float[dim][dim];
		for (int i = 0; i < dim; i++)
			for (int j = row_ptr.get(i); j < row_ptr.get(i + 1); j++)
				matrix[i][this.col_ind.get(j)] = this.val.get(j);
		return matrix;
	}
}
