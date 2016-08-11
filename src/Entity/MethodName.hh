<?hh // strict

namespace Ivyhjk\Xml\Entity;

use DOMElement;
use DOMDocument;
use SimpleXMLElement;
use Ivyhjk\Xml\Exception\InvalidNodeException;

/**
 * <methodName> tag concrete class.
 *
 * @since v1.0.0
 * @version v1.0.0
 * @package Ivyhjk\Xml\Entity
 * @author Elvis Munoz <elvis.munoz.f@gmail.com>
 * @copyright Copyright (c) 2016, Elvis Munoz
 * @license https://opensource.org/licenses/MIT MIT License
 */
class MethodName extends Tag
{
    /**
     * The XML tag name
     *
     * @var string
     */
    const string TAG_NAME = 'methodName';

    /**
     * Description of what this does.
     *
     * @param mixed $myVar Short description
     *
     * @return void
     */
    public function __construct(private string $name, DOMDocument $document) : void
    {
        parent::__construct($document);
    }

    /**
     * Get the tag/DOMElement name.
     *
     * @return string
     */
    public function getName() : string
    {
        return $this->name;
    }

    /**
     * Get the <methodCall> tag as a DOMelement.
     *
     * @return DOMElement
     */
    public function getElement() : DOMElement
    {
        $element = $this
            ->getDocument()
            ->createElement('methodName', $this->getName());

        return $element;
    }

    /**
     * Description of what this does.
     *
     * @param mixed $myVar Short description
     *
     * @return Ivyhjk\Xml\Entity\MethodName
     */
    public static function fromNode(SimpleXMLElement $node, DOMDocument $document) : MethodName
    {
        $name = $node->getName();

        if ($name !== 'methodName') {
            throw new InvalidNodeException();
        }

        $methodName = (string) $node;

        return new MethodName($methodName, $document);
    }
}
