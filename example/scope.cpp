/*	Graphics Library of Views (GLV) - GUI Building Toolkit
	See COPYRIGHT file for authors and license information */
/*
Example: 
*/

#include "example.h"

namespace glv{

struct DRFunction1D : public DataRenderer{
	void onDrawElements(draw::GraphicBuffers& b, const Data& d, const Indexer& i){

		prim(draw::LineStrip);
		draw::stroke(2);

		Indexer j(i.size());

		while(j()){
			double x = j[0];
			double y = d.at<double>(0, j[0]);

			b.addVertex2(x, y);
		}
	}
};

struct DRFunction2D : public DataRenderer{
	void onDrawElements(draw::GraphicBuffers& b, const Data& d, const Indexer& i){
		if(d.size(0) < 2) return;

		prim(draw::LineStrip);
		draw::stroke(2);
		while(i()){
			double x = d.at<double>(0, i[0], i[1]);
			double y = d.at<double>(1, i[0], i[1]);

			b.addVertex2(x, y);
		}
	}
};

struct MyGLV : GLV {

	MyGLV(): phase1(0), phase2(0){
		model().resize(Data::DOUBLE, 3, 16);
	}

	void onAnimate(double dt, GLV& g){
		const float pi = 3.141593;

		phase1 += 0.0037; while(phase1 > 2*pi){ phase1 -= 2*pi; }
		phase2 += 0.0031; while(phase2 > 2*pi){ phase2 -= 2*pi; }
		
		Data& d = model();
		Indexer i(d.size(1));
		
		while(i()){
			float f = i.frac(0);
			float p = f*2*pi;

			float c1 = cos( 1*p+phase1);
			float s1 = sin( 1*p+phase1);
			float c2 = cos(10*p+phase2);
			float s2 = sin(10*p+phase2);
			
			float x = s2 * c1;
			float y = s2 * s1;
			float z = c2;

			d.assign(x, 0, i[0], i[1]);
			d.assign(y, 1, i[0], i[1]);
			d.assign(z, 2, i[0], i[1]);
		}
	}

	double phase1, phase2;
};

}

int main(){

	// Create the Views
	MyGLV top;
	double d = 800;
	DataPlot<DRFunction1D> v1__(Rect(    0,0*d/8, d,d/8));
	DataPlot<DRFunction1D> v_1_(Rect(    0,1*d/8, d,d/8));
	DataPlot<DRFunction1D> v__1(Rect(    0,2*d/8, d,d/8));
	DataPlot<DRFunction2D> v11_(Rect(0*d/3,3*d/8, d/3,d/3));
	DataPlot<DRFunction2D> v_11(Rect(1*d/3,3*d/8, d/3,d/3));
	DataPlot<DRFunction2D> v1_1(Rect(2*d/3,3*d/8, d/3,d/3));

	Data& data = top.model();
	v1__.model() = data.slice(0).stride(data.size(0)).shape(1, data.size(1,2));
	v_1_.model() = data.slice(1).stride(data.size(0)).shape(1, data.size(1,2));
	v__1.model() = data.slice(2).stride(data.size(0)).shape(1, data.size(1,2));
	v11_.model() = data.slice(0).shape(data.shape(), 3);
	v_11.model() = data.slice(1).shape(data.shape(), 3);
	v1_1.model() = data.slice(1).shape(data.shape(), 3);

	//v11_.model().print();

	v1__.preserveAspect(false);
	v_1_.preserveAspect(false);
	v__1.preserveAspect(false);

	v1__.range(0, 0, data.size(1,2)).range(1, -1.2, 1.2);
	v_1_.range(0, 0, data.size(1,2)).range(1, -1.2, 1.2);
	v__1.range(0, 0, data.size(1,2)).range(1, -1.2, 1.2);

	v11_.range(1.2);
	v_11.range(1.2);
	v1_1.range(1.2);

	top << v1__ << v_1_ << v__1 << v11_ << v_11 << v1_1;

	Window win(0,0, "DataPlots", &top);
	win.fit();
	Application::run();
}

