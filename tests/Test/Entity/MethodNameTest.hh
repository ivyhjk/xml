<?hh // strict

namespace Ivyhjk\Xml\Test\Entity;

use DOMDocument;
use SimpleXMLElement;
use Ivyhjk\Xml\Entity\MethodName;
use Ivyhjk\Xml\Exception\InvalidNodeException;

/**
 * Test the method name correct workflow.
 *
 * @since v1.0.0
 * @version v1.0.0
 * @package Ivyhjk\Xml\Test\Entity
 * @author Elvis Munoz <elvis.munoz.f@gmail.com>
 * @copyright Copyright (c) 2016, Elvis Munoz
 * @license https://opensource.org/licenses/MIT MIT License
 */
/* HH_FIXME[4123] */ /* HH_FIXME[2049] */
class MethodNameTest extends \PHPUnit_Framework_TestCase
{
    /**
     * Test the correct element workflow.
     *
     * @return void
     */
    public function testElement() : void
    {
        $document = new DOMDocument();

        $methodName = new MethodName('foo', $document);

        $document->appendChild($methodName->getElement());

        $xml = preg_replace('/\n/', '', $document->saveXML());

        $expectedXml = '<?xml version="1.0"?><methodName>foo</methodName>';

        $this->assertEquals($expectedXml, $xml);
    }

    /**
     * Test the fromNode method when an invalid node is sent.
     *
     * @return void
     */
    public function testFromNodeInvalidNode() : void
    {
        $node = new SimpleXMLElement('<invalidTag/>');

        $this->expectException(InvalidNodeException::class);
        MethodName::fromNode($node, new DOMDocument());
    }

    /**
     * Test the correct workflow for fromNode method.
     *
     * @return void
     */
    public function testFromNode() : void
    {
        $node = new SimpleXMLElement('
            <methodName>MyMethod</methodName>
        ');

        $methodName = MethodName::fromNode($node, new DOMDocument());

        $this->assertSame('MyMethod', $methodName->getName());
    }
}
