#pragma warning( disable: 4067 )

#include "tt_Gene.h"
#include "tt_Factory.hxx"
#include "tt_Heatmap_Roamer.h"
#include "tt_Parameterized_Quadrilateral_Affine.h"
#include "tt_OpenCV_Image.h"

namespace foundations
{

	template<class ...T>
	struct Gene<T...>::Secret_Data
	{
		int kek_;
	};

	template<class ...T>
	Gene<T...>::Gene():secret_data_(new Secret_Data()) {
			LOG(foundations::logDEBUG4)<< "Gene" << " constructor called.";
	}


	template<class ...T>
	void Gene<T...>::set_parameters_internal(const Parameters_Type& parameters)
	{
		LOG(logDEBUG2) << "Defining transform with parameters:";
		this->define_transform(parameters);
		LOG(logDEBUG2) << "Defining transform with parameters done.";
	}

	template<class ...T>
	Range_Type Gene<T...>::get_range_internal() const
	{
		Range_Type output;
		using TL = List<T...>;
		foo_impl<TL>(output);
		//if (Has<Parameterized_Quadrilateral_Affine>)
		//	try_get_range<Parameterized_Quadrilateral_Affine>(output);
		//else
		//	try_get_range<Parameterized_Rectangular_Affine>(output);

		//try_get_range<Heatmap_Roamer>(output);
		return output;
	}


	// composite
	// Secret
	//{
	//	Heatmap,
	//	Rotation,
	//  Transform,
	//  Scale.
	//}

	// void set_genetic_code()
  //{
  //   loops through the genes and tries to cast them to Composite_Aware
  //   vector copy of the other genes
  //	 send other genes to the composite aware
  //}

	//when modifier call set_genetic_code 
	// class Composite_Aware : public Parameterized_Model(set and get_other_genes)
	// class Modifier: public Composite_Aware
	// class Heatmap : public Modifier
	// modify(self_code, gene_to_modify)
	// class Scale : public Modifier



	template<class ...T>
	void Gene<T...>::reset_parameters()
	{
		Parameters_Type parameters;
		Range_Type range = get_range_internal();
		for (auto el : range)
			parameters.push_back(el.first);
		try
		{
			this->set_parameters(parameters);
		}
		catch (...) {}
	}

	template<class ...T>
	void Gene<T...>::define_transform(const Parameters_Type & parameters)
	{
		using TL = List<T...>;
		lol<TL>(parameters);
		//if (Has<Parameterized_Quadrilateral_Affine>)
		//{
		//	Parameters_Type tmp;
		//	for (int i = 0; i < 5; i++)
		//		tmp.push_back(parameters[i]);
		//	try_define_transform<Parameterized_Quadrilateral_Affine>(tmp);
		//}
		//else
		//{
		//	Parameters_Type tmp;
		//	for (int i = 0; i < 4; i++)
		//		tmp.push_back(parameters[i]);
		//	try_define_transform<Parameterized_Rectangular_Affine>(tmp);
		//}
		//if (Has<Heatmap_Roamer>)
		//{
		//	call_if_base<Heatmap_Roamer>(&Heatmap_Roamer::set_neighborhood, parameters[parameters.size() - 2]);
		//	call_if_base<Heatmap_Roamer>(&Heatmap_Roamer::set_threshold, parameters[parameters.size() - 1]);
		//}
	}

	template<class ...T>
	std::shared_ptr<Parameterized_Transform> Gene<T...>::generate() const
	{
		 auto output = Self::New();

		if (const auto rectangle = this->get_baseline_to_rectangle())
		{
			output->set_baseline_to_rectangle(*rectangle);
		}

		if (const auto rectangle = this->get_from_rectangle())
		{
			output->set_from_rectangle(*rectangle);
		}

		if (Has<Parameterized_Quadrilateral_Affine>)
		{
			do_job<Parameterized_Quadrilateral_Affine>(*output);
		}

		output->set_min_corner_range(this->get_min_corner_range());
		output->set_size_range(this->get_size_range());

		if (Has<Heatmap_Roamer>)
		{
			do_job<Heatmap_Roamer>(*output);
		}
		try
		{
			output->set_parameters(this->get_parameters());
		}
		catch (const std::runtime_error & error)
		{
			LOG(logWARNING) << "Warning setting parameters: " << error.what();
		}

		return output;

	}

	template<class ...T>
	Gene<T...>::~Gene()
	{
		LOG(foundations::logDEBUG4) <<
			"Gene" << " destructor called.";
	}

}
template __declspec(dllexport)
class ::foundations::Factory<foundations::Gene<foundations::Parameterized_Quadrilateral_Affine>>;
template __declspec(dllexport)
class ::foundations::Enable_Shared_From_This<foundations::Gene<foundations::Parameterized_Quadrilateral_Affine>>;

template __declspec(dllexport)
class ::foundations::Factory<foundations::Gene<foundations::Parameterized_Rectangular_Affine>>;
template __declspec(dllexport)
class ::foundations::Enable_Shared_From_This<foundations::Gene<foundations::Parameterized_Rectangular_Affine>>;

//template __declspec(dllexport)
//class ::foundations::Factory<foundations::Gene<foundations::Heatmap_Roamer>>;
//template __declspec(dllexport)
//class ::foundations::Enable_Shared_From_This<foundations::Gene<foundations::Heatmap_Roamer>>;

template __declspec(dllexport)
class ::foundations::Factory<foundations::Gene<foundations::Heatmap_Roamer, foundations::Parameterized_Quadrilateral_Affine>>;
template __declspec(dllexport)
class ::foundations::Enable_Shared_From_This<foundations::Gene<foundations::Heatmap_Roamer, foundations::Parameterized_Quadrilateral_Affine>>;
template __declspec(dllexport)
class ::foundations::Factory<foundations::Gene<foundations::Parameterized_Quadrilateral_Affine, foundations::Heatmap_Roamer>>;
template __declspec(dllexport)
class ::foundations::Enable_Shared_From_This<foundations::Gene<foundations::Parameterized_Quadrilateral_Affine, foundations::Heatmap_Roamer>>;

template __declspec(dllexport)
class ::foundations::Factory<foundations::Gene<foundations::Heatmap_Roamer, foundations::Parameterized_Rectangular_Affine>>;
template __declspec(dllexport)
class ::foundations::Enable_Shared_From_This<foundations::Gene<foundations::Heatmap_Roamer, foundations::Parameterized_Rectangular_Affine>>;
template __declspec(dllexport)
class ::foundations::Factory<foundations::Gene<foundations::Parameterized_Rectangular_Affine, foundations::Heatmap_Roamer>>;
template __declspec(dllexport)
class ::foundations::Enable_Shared_From_This<foundations::Gene<foundations::Parameterized_Rectangular_Affine, foundations::Heatmap_Roamer>>;

