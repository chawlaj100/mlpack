/**
 * @file activation_functions_test.cpp
 * @author Marcus Edel
 * @author Dhawal Arora
 *
 * Tests for the various activation functions.
 *
 * mlpack is free software; you may redistribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#include <mlpack/core.hpp>

#include <mlpack/methods/ann/layer/layer.hpp>
#include <mlpack/methods/ann/activation_functions/logistic_function.hpp>
#include <mlpack/methods/ann/activation_functions/identity_function.hpp>
#include <mlpack/methods/ann/activation_functions/softsign_function.hpp>
#include <mlpack/methods/ann/activation_functions/tanh_function.hpp>
#include <mlpack/methods/ann/activation_functions/rectifier_function.hpp>
#include <mlpack/methods/ann/activation_functions/softplus_function.hpp>
#include <mlpack/methods/ann/activation_functions/swish_function.hpp>
#include <mlpack/methods/ann/activation_functions/hard_sigmoid_function.hpp>
#include <mlpack/methods/ann/activation_functions/mish_function.hpp>
#include <mlpack/methods/ann/activation_functions/lisht_function.hpp>
#include <mlpack/methods/ann/activation_functions/gelu_function.hpp>
#include <mlpack/methods/ann/activation_functions/elliot_function.hpp>
#include <mlpack/methods/ann/activation_functions/elish_function.hpp>

#include <boost/test/unit_test.hpp>
#include "test_tools.hpp"

using namespace mlpack;
using namespace mlpack::ann;

BOOST_AUTO_TEST_SUITE(ActivationFunctionsTest);

// Generate dataset for activation function tests.
const arma::colvec activationData("-2 3.2 4.5 -100.2 1 -1 2 0");

/**
 * Implementation of the activation function test.
 *
 * @param input Input data used for evaluating the activation function.
 * @param target Target data used to evaluate the activation.
 *
 * @tparam ActivationFunction Activation function used for the check.
 */
template<class ActivationFunction>
void CheckActivationCorrect(const arma::colvec input,
                            const arma::colvec target)
{
  // Test the activation function using a single value as input.
  for (size_t i = 0; i < target.n_elem; i++)
  {
    BOOST_REQUIRE_CLOSE(ActivationFunction::Fn(input.at(i)),
        target.at(i), 1e-3);
  }

  // Test the activation function using the entire vector as input.
  arma::colvec activations;
  ActivationFunction::Fn(input, activations);
  for (size_t i = 0; i < activations.n_elem; i++)
  {
    BOOST_REQUIRE_CLOSE(activations.at(i), target.at(i), 1e-3);
  }
}

/**
 * Implementation of the activation function derivative test.
 *
 * @param input Input data used for evaluating the activation function.
 * @param target Target data used to evaluate the activation.
 *
 * @tparam ActivationFunction Activation function used for the check.
 */
template<class ActivationFunction>
void CheckDerivativeCorrect(const arma::colvec input,
                            const arma::colvec target)
{
  // Test the calculation of the derivatives using a single value as input.
  for (size_t i = 0; i < target.n_elem; i++)
  {
    BOOST_REQUIRE_CLOSE(ActivationFunction::Deriv(input.at(i)),
        target.at(i), 1e-3);
  }

  // Test the calculation of the derivatives using the entire vector as input.
  arma::colvec derivatives;
  ActivationFunction::Deriv(input, derivatives);
  for (size_t i = 0; i < derivatives.n_elem; i++)
  {
    BOOST_REQUIRE_CLOSE(derivatives.at(i), target.at(i), 1e-3);
  }
}

/**
 * Implementation of the activation function inverse test.
 *
 * @param input Input data used for evaluating the activation function.
 * @param target Target data used to evaluate the activation.
 *
 * @tparam ActivationFunction Activation function used for the check.
 */
template<class ActivationFunction>
void CheckInverseCorrect(const arma::colvec input)
{
    // Test the calculation of the inverse using a single value as input.
  for (size_t i = 0; i < input.n_elem; i++)
  {
    BOOST_REQUIRE_CLOSE(ActivationFunction::Inv(ActivationFunction::Fn(
        input.at(i))), input.at(i), 1e-3);
  }

  // Test the calculation of the inverse using the entire vector as input.
  arma::colvec activations;
  ActivationFunction::Fn(input, activations);
  ActivationFunction::Inv(activations, activations);

  for (size_t i = 0; i < input.n_elem; i++)
  {
    BOOST_REQUIRE_CLOSE(activations.at(i), input.at(i), 1e-3);
  }
}

/**
 * Implementation of the HardTanH activation function test. The function is
 * implemented as a HardTanH Layer in hard_tanh.hpp
 *
 * @param input Input data used for evaluating the HardTanH activation function.
 * @param target Target data used to evaluate the HardTanH activation.
 */
void CheckHardTanHActivationCorrect(const arma::colvec input,
                                    const arma::colvec target)
{
  HardTanH<> htf;

  // Test the activation function using the entire vector as input.
  arma::colvec activations;
  htf.Forward(input, activations);
  for (size_t i = 0; i < activations.n_elem; i++)
  {
    BOOST_REQUIRE_CLOSE(activations.at(i), target.at(i), 1e-3);
  }
}

/**
 * Implementation of the HardTanH activation function derivative test. The
 * derivative is implemented as HardTanH Layer in hard_tanh.hpp
 *
 * @param input Input data used for evaluating the HardTanH activation
 * function.
 * @param target Target data used to evaluate the HardTanH activation.
 */
void CheckHardTanHDerivativeCorrect(const arma::colvec input,
                                    const arma::colvec target)
{
  HardTanH<> htf;

  // Test the calculation of the derivatives using the entire vector as input.
  arma::colvec derivatives;

  // This error vector will be set to 1 to get the derivatives.
  arma::colvec error = arma::ones<arma::colvec>(input.n_elem);
  htf.Backward(input, error, derivatives);

  for (size_t i = 0; i < derivatives.n_elem; i++)
  {
    BOOST_REQUIRE_CLOSE(derivatives.at(i), target.at(i), 1e-3);
  }
}

/**
 * Implementation of the LeakyReLU activation function test. The function is
 * implemented as LeakyReLU layer in the file leaky_relu.hpp
 *
 * @param input Input data used for evaluating the LeakyReLU activation
 * function.
 * @param target Target data used to evaluate the LeakyReLU activation.
 */
void CheckLeakyReLUActivationCorrect(const arma::colvec input,
                                     const arma::colvec target)
{
  LeakyReLU<> lrf;

  // Test the activation function using the entire vector as input.
  arma::colvec activations;
  lrf.Forward(input, activations);
  for (size_t i = 0; i < activations.n_elem; i++)
  {
    BOOST_REQUIRE_CLOSE(activations.at(i), target.at(i), 1e-3);
  }
}

/**
 * Implementation of the LeakyReLU activation function derivative test.
 * The derivative function is implemented as LeakyReLU layer in the file
 * leaky_relu_layer.hpp
 *
 * @param input Input data used for evaluating the LeakyReLU activation
 * function.
 * @param target Target data used to evaluate the LeakyReLU activation.
 */
void CheckLeakyReLUDerivativeCorrect(const arma::colvec input,
                                     const arma::colvec target)
{
  LeakyReLU<> lrf;

  // Test the calculation of the derivatives using the entire vector as input.
  arma::colvec derivatives;

  // This error vector will be set to 1 to get the derivatives.
  arma::colvec error = arma::ones<arma::colvec>(input.n_elem);
  lrf.Backward(input, error, derivatives);
  for (size_t i = 0; i < derivatives.n_elem; i++)
  {
    BOOST_REQUIRE_CLOSE(derivatives.at(i), target.at(i), 1e-3);
  }
}

/**
 * Implementation of the ELU activation function test. The function is
 * implemented as ELU layer in the file elu.hpp
 *
 * @param input Input data used for evaluating the ELU activation function.
 * @param target Target data used to evaluate the ELU activation.
 */
void CheckELUActivationCorrect(const arma::colvec input,
                               const arma::colvec target)
{
  // Initialize ELU object with alpha = 1.0.
  ELU<> lrf(1.0);

  // Test the activation function using the entire vector as input.
  arma::colvec activations;
  lrf.Forward(input, activations);
  for (size_t i = 0; i < activations.n_elem; i++)
  {
    BOOST_REQUIRE_CLOSE(activations.at(i), target.at(i), 1e-3);
  }
}

/**
 * Implementation of the ELU activation function derivative test. The function
 * is implemented as ELU layer in the file elu.hpp
 *
 * @param input Input data used for evaluating the ELU activation function.
 * @param target Target data used to evaluate the ELU activation.
 */
void CheckELUDerivativeCorrect(const arma::colvec input,
                               const arma::colvec target)
{
  // Initialize ELU object with alpha = 1.0.
  ELU<> lrf(1.0);

  // Test the calculation of the derivatives using the entire vector as input.
  arma::colvec derivatives, activations;

  // This error vector will be set to 1 to get the derivatives.
  arma::colvec error = arma::ones<arma::colvec>(input.n_elem);
  lrf.Forward(input, activations);
  lrf.Backward(activations, error, derivatives);
  for (size_t i = 0; i < derivatives.n_elem; i++)
  {
    BOOST_REQUIRE_CLOSE(derivatives.at(i), target.at(i), 1e-3);
  }
}

/**
 * Implementation of the PReLU activation function test. The function
 * is implemented as PReLU layer in the file parametric_relu.hpp.
 *
 * @param input Input data used for evaluating the PReLU activation
 *   function.
 * @param target Target data used to evaluate the PReLU activation.
 */
void CheckPReLUActivationCorrect(const arma::colvec input,
                                 const arma::colvec target)
{
  PReLU<> prelu;

  // Test the activation function using the entire vector as input.
  arma::colvec activations;
  prelu.Forward(input, activations);
  for (size_t i = 0; i < activations.n_elem; i++)
  {
    BOOST_REQUIRE_CLOSE(activations.at(i), target.at(i), 1e-3);
  }
}

/**
 * Implementation of the PReLU activation function derivative test.
 * The function is implemented as PReLU layer in the file
 * parametric_relu.hpp
 *
 * @param input Input data used for evaluating the PReLU activation
 *   function.
 * @param target Target data used to evaluate the PReLU activation.
 */
void CheckPReLUDerivativeCorrect(const arma::colvec input,
                                 const arma::colvec target)
{
  PReLU<> prelu;

  // Test the calculation of the derivatives using the entire vector as input.
  arma::colvec derivatives;

  // This error vector will be set to 1 to get the derivatives.
  arma::colvec error = arma::ones<arma::colvec>(input.n_elem);
  prelu.Backward(input, error, derivatives);
  for (size_t i = 0; i < derivatives.n_elem; i++)
  {
    BOOST_REQUIRE_CLOSE(derivatives.at(i), target.at(i), 1e-3);
  }
}

/**
 * Implementation of the PReLU activation function gradient test.
 * The function is implemented as PReLU layer in the file
 * parametric_relu.hpp
 *
 * @param input Input data used for evaluating the PReLU activation
 *   function.
 * @param target Target data used to evaluate the PReLU gradient.
 */
void CheckPReLUGradientCorrect(const arma::colvec input,
                               const arma::colvec target)
{
  PReLU<> prelu;

  // Test the calculation of the derivatives using the entire vector as input.
  arma::colvec gradient;

  // This error vector will be set to 1 to get the gradient.
  arma::colvec error = arma::ones<arma::colvec>(input.n_elem);
  prelu.Gradient(input, error, gradient);
  BOOST_REQUIRE_EQUAL(gradient.n_rows, 1);
  BOOST_REQUIRE_EQUAL(gradient.n_cols, 1);
  BOOST_REQUIRE_CLOSE(gradient(0), target(0), 1e-3);
}

/**
 * Implementation of the Hard Shrink activation function test. The function is
 * implemented as Hard Shrink layer in the file hardshrink.hpp
 *
 * @param input Input data used for evaluating the Hard Shrink activation function.
 * @param target Target data used to evaluate the Hard Shrink activation.
 */
void CheckHardShrinkActivationCorrect(const arma::colvec input,
                                      const arma::colvec target)
{
  HardShrink<> hardshrink;

  // Test the activation function using the entire vector as input.
  arma::colvec activations;
  hardshrink.Forward(input, activations);
  for (size_t i = 0; i < activations.n_elem; i++)
  {
    BOOST_REQUIRE_CLOSE(activations.at(i), target.at(i), 1e-3);
  }
}

/**
 * Implementation of the HardShrink activation function derivative test.
 * The derivative function is implemented as HardShrink layer in the file
 * hardshrink.hpp
 *
 * @param input Input data used for evaluating the HardShrink activation
 * function.
 * @param target Target data used to evaluate the HardShrink activation.
 */
void CheckHardShrinkDerivativeCorrect(const arma::colvec input,
                                      const arma::colvec target)
{
  HardShrink<> hardshrink;

  // Test the calculation of the derivatives using the entire vector as input.
  arma::colvec derivatives;

  // This error vector will be set to 1 to get the derivatives.
  arma::colvec error = arma::ones<arma::colvec>(input.n_elem);
  hardshrink.Backward(input, error, derivatives);
  for (size_t i = 0; i < derivatives.n_elem; i++)
  {
    BOOST_REQUIRE_CLOSE(derivatives.at(i), target.at(i), 1e-3);
  }
}

/**
 * Implementation of the Soft Shrink activation function test. The function is
 * implemented as Soft Shrink layer in the file softshrink.hpp.
 *
 * @param input Input data used for evaluating the Soft Shrink activation
 * function.
 * @param target Target data used to evaluate the Soft Shrink activation.
 */
void CheckSoftShrinkActivationCorrect(const arma::colvec input,
                                      const arma::colvec target)
{
  SoftShrink<> softshrink;

  // Test the activation function using the entire vector as input.
  arma::colvec activations;
  softshrink.Forward(input, activations);
  for (size_t i = 0; i < activations.n_elem; i++)
  {
    BOOST_REQUIRE_CLOSE(activations.at(i), target.at(i), 1e-3);
  }
}

/**
 * Implementation of the Soft Shrink activation function derivative test.
 * The derivative function is implemented as Soft Shrink layer in the file
 * softshrink.hpp
 *
 * @param input Input data used for evaluating the Soft Shrink activation
 * function.
 * @param target Target data used to evaluate the Soft Shrink activation.
 */
void CheckSoftShrinkDerivativeCorrect(const arma::colvec input,
                                      const arma::colvec target)
{
  SoftShrink<> softshrink;

  // Test the calculation of the derivatives using the entire vector as input.
  arma::colvec derivatives;

  // This error vector will be set to 1 to get the derivatives.
  arma::colvec error = arma::ones<arma::colvec>(input.n_elem);
  softshrink.Backward(input, error, derivatives);
  for (size_t i = 0; i < derivatives.n_elem; i++)
  {
    BOOST_REQUIRE_CLOSE(derivatives.at(i), target.at(i), 1e-3);
  }
}

/**
 * Simple SELU activation test to check whether the mean and variance remain
 * invariant after passing normalized inputs through the function.
 */
BOOST_AUTO_TEST_CASE(SELUFunctionNormalizedTest)
{
  arma::mat input = arma::randn<arma::mat>(1000, 1);

  arma::mat output;

  SELU selu;

  selu.Forward(input, output);

  BOOST_REQUIRE_LE(arma::as_scalar(arma::abs(arma::mean(input) -
      arma::mean(output))), 0.1);

  BOOST_REQUIRE_LE(arma::as_scalar(arma::abs(arma::var(input) -
      arma::var(output))), 0.1);
}

/**
 * Simple SELU activation test to check whether the mean and variance
 * vary significantly after passing unnormalized inputs through the function.
 */
BOOST_AUTO_TEST_CASE(SELUFunctionUnnormalizedTest)
{
  const arma::colvec input("5.96402758 0.9966824 0.99975321 1 \
                            7.76159416 -0.76159416 0.96402758 8");

  arma::mat output;

  SELU selu;

  selu.Forward(input, output);

  BOOST_REQUIRE_GE(arma::as_scalar(arma::abs(arma::mean(input) -
      arma::mean(output))), 0.1);

  BOOST_REQUIRE_GE(arma::as_scalar(arma::abs(arma::var(input) -
      arma::var(output))), 0.1);
}

/**
 * Simple SELU derivative test to check whether the derivatives
 * produced by the activation function are correct.
 *
 */
BOOST_AUTO_TEST_CASE(SELUFunctionDerivativeTest)
{
  arma::mat input = arma::ones<arma::mat>(1000, 1);

  arma::mat error = arma::ones<arma::mat>(input.n_elem, 1);

  arma::mat derivatives, activations;

  SELU selu;

  selu.Forward(input, activations);
  selu.Backward(activations, error, derivatives);

  BOOST_REQUIRE_LE(arma::as_scalar(arma::abs(arma::mean(derivatives) -
      selu.Lambda())), 10e-4);

  input.fill(-1);

  selu.Forward(input, activations);
  selu.Backward(activations, error, derivatives);

  BOOST_REQUIRE_LE(arma::as_scalar(arma::abs(arma::mean(derivatives) -
      selu.Lambda() * selu.Alpha() - arma::mean(activations))), 10e-4);
}

/**
 * Implementation of the CELU activation function test. The function is
 * implemented as CELU layer in the file celu.hpp.
 *
 * @param input Input data used for evaluating the CELU activation function.
 * @param target Target data used to evaluate the CELU activation.
 */
void CheckCELUActivationCorrect(const arma::colvec input,
                                const arma::colvec target)
{
  // Initialize CELU object with alpha = 1.0.
  CELU<> lrf(1.0);

  // Test the activation function using the entire vector as input.
  arma::colvec activations;
  lrf.Forward(input, activations);
  for (size_t i = 0; i < activations.n_elem; i++)
  {
    BOOST_REQUIRE_CLOSE(activations.at(i), target.at(i), 1e-3);
  }
}

/**
 * Implementation of the CELU activation function derivative test. The function
 * is implemented as CELU layer in the file celu.hpp.
 *
 * @param input Input data used for evaluating the CELU activation function.
 * @param target Target data used to evaluate the CELU activation.
 */
void CheckCELUDerivativeCorrect(const arma::colvec input,
                                const arma::colvec target)
{
  // Initialize CELU object with alpha = 1.0.
  CELU<> lrf(1.0);

  // Test the calculation of the derivatives using the entire vector as input.
  arma::colvec derivatives, activations;

  // This error vector will be set to 1 to get the derivatives.
  arma::colvec error = arma::ones<arma::colvec>(input.n_elem);
  lrf.Forward(input, activations);
  lrf.Backward(activations, error, derivatives);
  for (size_t i = 0; i < derivatives.n_elem; i++)
  {
    BOOST_REQUIRE_CLOSE(derivatives.at(i), target.at(i), 1e-3);
  }
}

/**
 * Basic test of the tanh function.
 */
BOOST_AUTO_TEST_CASE(TanhFunctionTest)
{
  const arma::colvec desiredActivations("-0.96402758 0.9966824 0.99975321 -1 \
                                         0.76159416 -0.76159416 0.96402758 0");

  const arma::colvec desiredDerivatives("0.07065082 0.00662419 0.00049352 0 \
                                         0.41997434 0.41997434 0.07065082 1");

  CheckActivationCorrect<TanhFunction>(activationData, desiredActivations);
  CheckDerivativeCorrect<TanhFunction>(desiredActivations, desiredDerivatives);
  CheckInverseCorrect<TanhFunction>(desiredActivations);
}

/**
 * Basic test of the logistic function.
 */
BOOST_AUTO_TEST_CASE(LogisticFunctionTest)
{
  const arma::colvec desiredActivations("1.19202922e-01 9.60834277e-01 \
                                         9.89013057e-01 3.04574e-44 \
                                         7.31058579e-01 2.68941421e-01 \
                                         8.80797078e-01 0.5");

  const arma::colvec desiredDerivatives("0.10499359 0.03763177 0.01086623 \
                                         3.04574e-44 0.19661193 0.19661193 \
                                         0.10499359 0.25");

  CheckActivationCorrect<LogisticFunction>(activationData, desiredActivations);
  CheckDerivativeCorrect<LogisticFunction>(desiredActivations,
                                           desiredDerivatives);
  CheckInverseCorrect<LogisticFunction>(activationData);
}

/**
 * Basic test of the softsign function.
 */
BOOST_AUTO_TEST_CASE(SoftsignFunctionTest)
{
  const arma::colvec desiredActivations("-0.66666667 0.76190476 0.81818182 \
                                         -0.99011858 0.5 -0.5 0.66666667 0");

  const arma::colvec desiredDerivatives("0.11111111 0.05668934 0.03305785 \
                                         9.7642e-05 0.25 0.25 0.11111111 1");

  CheckActivationCorrect<SoftsignFunction>(activationData, desiredActivations);
  CheckDerivativeCorrect<SoftsignFunction>(desiredActivations,
                                           desiredDerivatives);
  CheckInverseCorrect<SoftsignFunction>(desiredActivations);
}

/**
 * Basic test of the identity function.
 */
BOOST_AUTO_TEST_CASE(IdentityFunctionTest)
{
  const arma::colvec desiredDerivatives = arma::ones<arma::colvec>(
      activationData.n_elem);

  CheckActivationCorrect<IdentityFunction>(activationData, activationData);
  CheckDerivativeCorrect<IdentityFunction>(activationData, desiredDerivatives);
}

/**
 * Basic test of the rectifier function.
 */
BOOST_AUTO_TEST_CASE(RectifierFunctionTest)
{
  const arma::colvec desiredActivations("0 3.2 4.5 0 1 0 2 0");

  const arma::colvec desiredDerivatives("0 1 1 0 1 0 1 0");

  CheckActivationCorrect<RectifierFunction>(activationData, desiredActivations);
  CheckDerivativeCorrect<RectifierFunction>(desiredActivations,
                                            desiredDerivatives);
}

/**
 * Basic test of the LeakyReLU function.
 */
BOOST_AUTO_TEST_CASE(LeakyReLUFunctionTest)
{
  const arma::colvec desiredActivations("-0.06 3.2 4.5 -3.006 \
                                         1 -0.03 2 0");

  const arma::colvec desiredDerivatives("0.03 1 1 0.03 \
                                         1 0.03 1 1");

  CheckLeakyReLUActivationCorrect(activationData, desiredActivations);
  CheckLeakyReLUDerivativeCorrect(desiredActivations, desiredDerivatives);
}

/**
 * Basic test of the HardTanH function.
 */
BOOST_AUTO_TEST_CASE(HardTanHFunctionTest)
{
  const arma::colvec desiredActivations("-1 1 1 -1 \
                                         1 -1 1 0");

  const arma::colvec desiredDerivatives("0 0 0 0 \
                                         1 1 0 1");

  CheckHardTanHActivationCorrect(activationData, desiredActivations);
  CheckHardTanHDerivativeCorrect(activationData, desiredDerivatives);
}

/**
 * Basic test of the ELU function.
 */
BOOST_AUTO_TEST_CASE(ELUFunctionTest)
{
  const arma::colvec desiredActivations("-0.86466471 3.2 4.5 -1.0 \
                                         1 -0.63212055 2 0");

  const arma::colvec desiredDerivatives("0.13533529 1 1 0 \
                                         1 0.36787945 1 1");

  CheckELUActivationCorrect(activationData, desiredActivations);
  CheckELUDerivativeCorrect(activationData, desiredDerivatives);
}

/**
 * Basic test of the softplus function.
 */
BOOST_AUTO_TEST_CASE(SoftplusFunctionTest)
{
  const arma::colvec activationData("-2 3.2 4.5 -100.2 1 -1 2 0 1000 10000");

  const arma::colvec desiredActivations("0.12692801 3.23995333 4.51104774 \
                                         0 1.31326168 0.31326168 2.12692801 \
                                         0.69314718 1000 10000");

  const arma::colvec desiredDerivatives("0.53168946 0.96231041 0.98913245 \
                                         0.5 0.78805844 0.57768119 0.89349302\
                                         0.66666666 1 1");

  CheckActivationCorrect<SoftplusFunction>(activationData, desiredActivations);
  CheckDerivativeCorrect<SoftplusFunction>(desiredActivations,
                                           desiredDerivatives);
  CheckInverseCorrect<SoftplusFunction>(desiredActivations);
}

/**
 * Basic test of the PReLU function.
 */
BOOST_AUTO_TEST_CASE(PReLUFunctionTest)
{
  const arma::colvec desiredActivations("-0.06 3.2 4.5 -3.006 \
                                         1 -0.03 2 0");

  const arma::colvec desiredDerivatives("0.03 1 1 0.03 \
                                         1 0.03 1 1");
  const arma::colvec desiredGradient("-103.2");

  CheckPReLUActivationCorrect(activationData, desiredActivations);
  CheckPReLUDerivativeCorrect(desiredActivations, desiredDerivatives);
  CheckPReLUGradientCorrect(activationData, desiredGradient);
}

/**
 * Basic test of the CReLU function.
 */
BOOST_AUTO_TEST_CASE(CReLUFunctionTest)
{
  const arma::colvec desiredActivations("0 3.2 4.5 0 \
                                         1 0 2 0 2 0 0 \
                                         100.2 0 1 0 0");

  const arma::colvec desiredDerivatives("0 0 0 0 \
                                         0 0 0 0");
  CReLU<> crelu;
  // Test the activation function using the entire vector as input.
  arma::colvec activations;
  crelu.Forward(activationData, activations);
  arma::colvec derivatives;
  // This error vector will be set to 1 to get the derivatives.
  arma::colvec error = arma::ones<arma::colvec>(desiredActivations.n_elem);
  crelu.Backward(desiredActivations, error, derivatives);
  for (size_t i = 0; i < activations.n_elem; i++)
  {
    BOOST_REQUIRE_CLOSE(activations.at(i), desiredActivations.at(i), 1e-3);
  }
  for (size_t i = 0; i < derivatives.n_elem; i++)
  {
    BOOST_REQUIRE_CLOSE(derivatives.at(i), desiredDerivatives.at(i), 1e-3);
  }
}

/**
 * Basic test of the swish function.
 */
BOOST_AUTO_TEST_CASE(SwishFunctionTest)
{
  // Hand-calculated values using Python interpreter.
  const arma::colvec desiredActivations("-0.238405 3.07466 4.45055 \
                                         -3.05183208657e-42 0.731058 -0.26894 \
                                         1.76159 0");

  const arma::colvec desiredDerivatives("0.3819171 1.0856295 1.039218 \
                                         0.5 0.83540367 0.3671335 1.073787\
                                         0.5");

  CheckActivationCorrect<SwishFunction>(activationData, desiredActivations);
  CheckDerivativeCorrect<SwishFunction>(desiredActivations,
                                        desiredDerivatives);
}

/**
 * Basic test of the hard sigmoid function.
 */
BOOST_AUTO_TEST_CASE(HardSigmoidFunctionTest)
{
  // Hand-calculated values using Python interpreter.
  const arma::colvec desiredActivations("0.1 1 1 \
                                         0 0.7 0.3 \
                                         0.9 0.5");

  const arma::colvec desiredDerivatives("0.2 0.0 0.0 \
                                         0.0 0.2 0.2 0.2\
                                         0.2");

  CheckActivationCorrect<HardSigmoidFunction>(activationData,
                                              desiredActivations);
  CheckDerivativeCorrect<HardSigmoidFunction>(desiredActivations,
                                              desiredDerivatives);
}

/**
 * Basic test of the Mish function.
 */
BOOST_AUTO_TEST_CASE(MishFunctionTest)
{
  // Calculated using tfa.activations.mish().
  // where tfa is tensorflow_addons.
  const arma::colvec desiredActivations("-0.25250152 3.1901977 \
                                         4.498914 -3.05183208e-42 0.86509836 \
                                         -0.30340138 1.943959 0");

  const arma::colvec desiredDerivatives("0.4382387  1.0159768849 \
                                         1.0019108 0.6 \
                                         1.0192586  0.40639898 \
                                         1.0725079  0.6");

  CheckActivationCorrect<MishFunction>(activationData,
                                       desiredActivations);
  CheckDerivativeCorrect<MishFunction>(desiredActivations,
                                       desiredDerivatives);
}

/**
 * Basic test of the LiSHT function.
 */
BOOST_AUTO_TEST_CASE(LiSHTFunctionTest)
{
  // Calculated using tfa.activations.LiSHT().
  // where tfa is tensorflow_addons.
  const arma::colvec desiredActivations("1.928055 3.189384 \
                                         4.4988894 100.2 0.7615942 \
                                         0.7615942 1.9280552 0");

  const arma::colvec desiredDerivatives("1.1150033 1.0181904 \
                                         1.001978 1.0 \
                                         1.0896928 1.0896928 \
                                         1.1150033 0.0");

  CheckActivationCorrect<LiSHTFunction>(activationData,
                                        desiredActivations);
  CheckDerivativeCorrect<LiSHTFunction>(desiredActivations,
                                        desiredDerivatives);
}

/**
 * Basic test of the GELU function.
 */
BOOST_AUTO_TEST_CASE(GELUFunctionTest)
{
  // Calculated using torch.nn.gelu().
  const arma::colvec desiredActivations("-0.0454023 3.1981304 \
                                         4.5 -0.0 0.84119199 \
                                         -0.158808 1.954597694 0.0");

  const arma::colvec desiredDerivatives("0.4637992 1.0065302 \
                                         1.0000293 0.5 1.03513446 \
                                         0.37435387 1.090984 0.5");

  CheckActivationCorrect<GELUFunction>(activationData,
                                       desiredActivations);
  CheckDerivativeCorrect<GELUFunction>(desiredActivations,
                                       desiredDerivatives);
}

/**
 * Basic test of the Hard Shrink function.
 */
BOOST_AUTO_TEST_CASE(HardShrinkFunctionTest)
{
  const arma::colvec desiredActivations("-2 3.2 4.5 -100.2 1 -1 2 0");

  const arma::colvec desiredDerivatives("1 1 1 1 1 1 1 0");

  CheckHardShrinkActivationCorrect(activationData,
                                   desiredActivations);
  CheckHardShrinkDerivativeCorrect(desiredActivations,
                                   desiredDerivatives);
}

/**
 * Basic test of the Elliot function.
 */
BOOST_AUTO_TEST_CASE(ElliotFunctionTest)
{
  // Calculated using PyTorch tensor.
  const arma::colvec desiredActivations("-0.66666667 0.76190476 0.81818182 \
                                         -0.99011858 0.5 -0.5 \
                                          0.66666667 0.0 ");

  const arma::colvec desiredDerivatives("0.36 0.32213294 0.3025 \
                                         0.25248879 0.44444444 \
                                         0.44444444 0.36 1.0 ");

  CheckActivationCorrect<ElliotFunction>(activationData,
                                         desiredActivations);
  CheckDerivativeCorrect<ElliotFunction>(desiredActivations,
                                         desiredDerivatives);
}

/**
 * Basic test of the EliSH function.
 */
BOOST_AUTO_TEST_CASE(ElishFunctionTest)
{
  // Manually-calculated using python-numpy module.
  const arma::colvec desiredActivations("-0.10307056 3.0746696 4.4505587 \
                                         -3.0457406e-44 0.731058578 \
                                         -0.1700034 1.76159415 0.0 ");

  const arma::colvec desiredDerivatives("0.4033889 1.0856292 \
                                         1.03921798 0.5 0.83540389 \
                                         0.34725726 1.07378804 0.5");

  CheckActivationCorrect<ElishFunction>(activationData,
                                        desiredActivations);
  CheckDerivativeCorrect<ElishFunction>(desiredActivations,
                                        desiredDerivatives);
}

/** 
 * Basic test of the Soft Shrink function.
 */
BOOST_AUTO_TEST_CASE(SoftShrinkFunctionTest)
{
  const arma::colvec desiredActivations("-1.5 2.7 4 -99.7 0.5 -0.5 1.5 0");

  const arma::colvec desiredDerivatives("1 1 1 1 1 1 1 0");

  CheckSoftShrinkActivationCorrect(activationData,
                                   desiredActivations);
  CheckSoftShrinkDerivativeCorrect(desiredActivations,
                                   desiredDerivatives);
}

/**
 * Basic test of the CELU activation function.
 */
BOOST_AUTO_TEST_CASE(CELUFunctionTest)
{
  const arma::colvec desiredActivations("-0.86466472 3.2 4.5 \
                                         -1 1 -0.63212056 2 0");

  const arma::colvec desiredDerivatives("0.42119275 1 1 \
                                         0.36787944 1 \
                                         0.5314636 1 1");

  CheckCELUActivationCorrect(activationData, desiredActivations);
  CheckCELUDerivativeCorrect(desiredActivations, desiredDerivatives);
}

BOOST_AUTO_TEST_SUITE_END();
