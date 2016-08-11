<?hh // strict

namespace Ivyhjk\Xml\Exception;

/**
 * Handle XML library exceptions
 *
 * @since v1.0.0
 * @version v1.0.0
 * @package Ivyhjk\Xml\Exception
 * @author Elvis Munoz <elvis.munoz.f@gmail.com>
 * @copyright Copyright (c) 2016, Elvis Munoz
 * @license https://opensource.org/licenses/MIT MIT License
 */
class XmlException extends \Exception
{
    protected string $message = 'XML unknown error.';
}
